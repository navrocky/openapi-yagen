require_relative "test_helper"

class BinaryAndMultiResponseTest < Minitest::Test
  include RoutedApp

  def test_put_file_streams_the_raw_body
    put "/files/a", "0123".b, { "CONTENT_TYPE" => "application/octet-stream" }
    assert_equal 204, last_response.status
    assert_equal "0123", FILES_HANDLER.files["a"]
  end

  def test_put_file_rejects_a_mismatched_content_type_with_415
    put "/files/a", "{}", { "CONTENT_TYPE" => "text/plain" }
    assert_equal 415, last_response.status
  end

  def test_put_file_rejects_a_missing_required_body_with_422
    put "/files/a"
    assert_equal 422, last_response.status
  end

  def test_echo_file_wildcard_media_type_and_optional_body
    post "/files/x/echo", "%PDF", { "CONTENT_TYPE" => "application/pdf; charset=binary" }
    assert_equal 200, last_response.status
    assert_equal "%PDF", last_response.body
    assert_equal "application/zip", last_response.content_type

    post "/files/x/echo", "hi", { "CONTENT_TYPE" => "text/plain" }
    assert_equal 415, last_response.status

    post "/files/x/echo"
    assert_equal "empty", last_response.body
  end

  def test_get_file_sends_content_disposition
    get "/files/hello"
    assert_equal 200, last_response.status
    assert_equal "hello, world", last_response.body
    assert_equal "application/octet-stream", last_response.content_type
    assert_match(/\Aattachment; filename="hello.bin"/, last_response.headers["Content-Disposition"])
  end

  def test_create_archive_picks_the_status_by_the_returned_response
    post "/jobs/archive?sync=true"
    assert_equal 200, last_response.status
    assert_equal "PKzip", last_response.body
    assert_equal "application/zip", last_response.content_type
    assert_match(/filename="archive.zip"/, last_response.headers["Content-Disposition"])

    post "/jobs/archive"
    assert_equal 202, last_response.status
    assert_equal({ "id" => "job-1" }, JSON.parse(last_response.body))

    post "/jobs/rebuild"
    assert_equal 204, last_response.status
  end

  def test_an_undeclared_response_status_is_an_error
    assert_raises(ArgumentError) { post "/jobs/ping" }
  end
end
