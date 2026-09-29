require "minitest/autorun"
require "action_controller"
require "action_controller/api"
require "rack/test"
require "securerandom"
require_relative "../generated/kitchensink"

# Bare ActionDispatch::Routing::RouteSet as the Rack app under test - deliberately no
# Rails::Application/full "rails" gem (see ../../README.md and the Gemfile) - a RouteSet is
# already a plain Rack app on its own (#call), and Kitchensink::Routes.draw's own `mapper`
# argument is exactly what `route_set.draw { ... }` yields (an ActionDispatch::Routing::Mapper),
# same object shape a real config/routes.rb hands it.
ROUTES = ActionDispatch::Routing::RouteSet.new

# Minimal in-memory fake implementations (see the sibling Tornado/Go/Kotlin server generators'
# own hand-written fake handlers) - each `include`s the real generated *HandlerInterface module,
# so a method this test suite forgets to override still raises NotImplementedError loudly rather
# than silently returning nil.
class FakePetsHandler
  include Kitchensink::PetsHandlerInterface

  NotFound = Class.new(StandardError)
  Pet = Struct.new(:id, :name, :tag, :notes, :status, :uuid, :adopted_on, :last_seen_at)

  def initialize
    @pets = { 1 => Pet.new(1, "Rex", "dog", nil, "available", nil, nil, nil) }
    @next_id = 2
  end

  def list_pets(limit: nil, tag: nil, tags: nil, session_id: nil)
    Kitchensink::Pets.from_h(@pets.values.map { |p| { "id" => p.id, "name" => p.name, "tag" => p.tag, "status" => p.status } })
  end

  def create_pet(body:)
    pet = Pet.new(@next_id, body.name, body.tag, body.notes, body.status, nil, nil, nil)
    @next_id += 1
    @pets[pet.id] = pet
    Kitchensink::Pet.from_object(pet)
  end

  def get_pet_by_id(pet_id:)
    pet = @pets.fetch(pet_id.to_i) { raise NotFound }
    Kitchensink::Pet.from_object(pet)
  end

  def delete_pet(pet_id:, bearer_auth:)
    @pets.delete(pet_id.to_i)
    nil
  end

  def upload_pet_avatar(pet_id:, body:)
    body.read # echoes the raw bytes back
  end

  def get_named_tag(pet_id:)
    Kitchensink::NamedPet.new(id: pet_id.to_i, name: "Rex", species: "dog")
  end

  def set_pet_notes(pet_id:, body:)
    body
  end

  def upload_pet_photo(pet_id:, body:)
    nil
  end

  def rate_pet(pet_id:, x_request_id:, api_key_auth:, body:)
    nil
  end

  def subscribe_to_pet(pet_id:, body:)
    nil
  end
end

class FakeWidgetsHandler
  include Kitchensink::WidgetsHandlerInterface

  def list_widgets(status: nil, x_client_version: nil)
    Kitchensink::Widgets.from_h([])
  end

  def create_widget(body:)
    body
  end

  def get_shape(shape_id:)
    Kitchensink::Circle.new(shape_type: "circle", radius: 1.0)
  end

  def archive_widget(widget_id:, api_key_auth:, oauth_2_auth:, bearer_auth:)
    nil
  end

  def favorite_widget(widget_id:, oauth_2_auth:, api_key_auth:)
    nil
  end
end

# Only ever reads/writes raw bodies.
class FakeFilesHandler
  include Kitchensink::FilesHandlerInterface

  attr_reader :files

  def initialize
    @files = { "hello" => "hello, world".b }
  end

  def get_file(file_id:)
    Kitchensink::Runtime::BinaryContent.new(data: @files.fetch(file_id), name: "#{file_id}.bin")
  end

  def put_file(file_id:, body:)
    # Streamed through IO.copy_stream rather than read whole.
    out = StringIO.new("".b)
    IO.copy_stream(body, out)
    @files[file_id] = out.string
    nil
  end

  def echo_file(file_id:, body: nil)
    body.nil? ? "empty" : body.read
  end
end

class FakeJobsHandler
  include Kitchensink::JobsHandlerInterface

  def create_archive(sync: nil)
    if sync
      Kitchensink::Runtime::Response.new(status: 200, body: Kitchensink::Runtime::BinaryContent.new(data: "PKzip", name: "archive.zip"))
    else
      Kitchensink::Runtime::Response.new(status: 202, body: Kitchensink::Job.new(id: "job-1"))
    end
  end

  def rebuild
    Kitchensink::Runtime::Response.new(status: 204)
  end

  def ping
    Kitchensink::Runtime::Response.new(status: 201) # undeclared - must not be sent as-is
  end
end

FILES_HANDLER = FakeFilesHandler.new

ROUTES.draw do
  Kitchensink::Routes.draw(
    self,
    handlers: { pets: FakePetsHandler.new, widgets: FakeWidgetsHandler.new, files: FILES_HANDLER, jobs: FakeJobsHandler.new },
  )
end

module RoutedApp
  include Rack::Test::Methods

  def app
    ROUTES
  end
end
