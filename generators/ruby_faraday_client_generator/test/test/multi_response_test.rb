require_relative "test_helper"

class MultiResponseTest < Minitest::Test
  def stubbed_connection
    stubs = Faraday::Adapter::Test::Stubs.new
    yield stubs
    conn = Faraday.new { |f| f.adapter :test, stubs }
    [conn, stubs]
  end

  def test_create_archive_decodes_the_variant_matching_the_response_status
    conn, = stubbed_connection do |stub|
      stub.post("/jobs/archive?sync=true") { [200, { "Content-Type" => "application/zip" }, "PKzip"] }
      stub.post("/jobs/archive") { [202, { "Content-Type" => "application/json" }, '{"id":"job-1"}'] }
    end
    api = Kitchensink::JobsClient.new(connection: conn)

    sync = api.create_archive(sync: true)
    assert_equal 200, sync.status
    assert_equal "PKzip", sync.body
    assert_equal Encoding::ASCII_8BIT, sync.body.encoding

    accepted = api.create_archive
    assert_equal 202, accepted.status
    assert_instance_of Kitchensink::Job, accepted.body
    assert_equal "job-1", accepted.body.id
  end

  def test_bodyless_variants_and_undeclared_statuses
    status = 204
    conn, = stubbed_connection do |stub|
      stub.post("/jobs/rebuild") { [status, {}, ""] }
      stub.post("/jobs/ping") { [status, {}, ""] }
    end
    api = Kitchensink::JobsClient.new(connection: conn)

    rebuilt = api.rebuild
    assert_equal 204, rebuilt.status
    assert_nil rebuilt.body

    status = 200
    assert_equal 200, api.ping.status
    status = 201
    error = assert_raises(Kitchensink::Runtime::ApiError) { api.ping }
    assert_equal 201, error.status
  end

  def test_a_wildcard_request_media_type_is_sent_as_application_octet_stream
    conn, stubs = stubbed_connection do |stub|
      stub.post("/files/x/echo") do |env|
        assert_equal "application/octet-stream", env.request_headers["Content-Type"]
        [200, { "Content-Type" => "application/zip" }, "x"]
      end
    end
    Kitchensink::FilesClient.new(connection: conn).echo_file(file_id: "x", body: "x")
    stubs.verify_stubbed_calls
  end
end
