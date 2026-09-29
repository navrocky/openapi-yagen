require_relative "concern_test_helper"

class ConcernBinaryAndMultiResponseTest < Minitest::Test
  include Rack::Test::Methods

  def app
    CONCERN_ROUTES
  end

  def test_raw_body_and_content_type_check
    put "/files/a", "0123".b, { "CONTENT_TYPE" => "application/octet-stream" }
    assert_equal 204, last_response.status
    put "/files/a", "x", { "CONTENT_TYPE" => "text/plain" }
    assert_equal 415, last_response.status
  end

  def test_multiple_success_responses
    post "/jobs/archive?sync=true"
    assert_equal 200, last_response.status
    assert_equal "PKzip", last_response.body
    assert_equal "application/zip", last_response.content_type

    post "/jobs/archive"
    assert_equal 202, last_response.status

    post "/jobs/rebuild"
    assert_equal 204, last_response.status
  end
end
