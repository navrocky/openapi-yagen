package com.example.kitchensink.client

import com.example.kitchensink.client.apis.CreateArchiveResponse
import com.example.kitchensink.client.apis.FilesApi
import com.example.kitchensink.client.apis.JobsApi
import com.example.kitchensink.client.apis.PingResponse
import com.example.kitchensink.client.apis.RebuildResponse
import com.example.kitchensink.client.models.Job
import com.example.kitchensink.client.support.buildTestClient
import io.ktor.client.engine.mock.respond
import io.ktor.client.plugins.ResponseException
import io.ktor.client.request.HttpRequestData
import io.ktor.http.HttpHeaders
import io.ktor.http.HttpStatusCode
import io.ktor.http.headersOf
import kotlinx.coroutines.test.runTest
import kotlin.test.Test
import kotlin.test.assertContentEquals
import kotlin.test.assertEquals
import kotlin.test.assertFailsWith
import kotlin.test.assertIs
import kotlin.test.assertSame

class MultiResponseTest {
    @Test
    fun `createArchive decodes the variant matching the response status`() = runTest {
        val client = buildTestClient { request ->
            if (request.url.parameters["sync"] == "true") {
                respond("PKzip".encodeToByteArray(), HttpStatusCode.OK, headersOf(HttpHeaders.ContentType, "application/zip"))
            } else {
                respond("""{"id":"job-1"}""", HttpStatusCode.Accepted, headersOf(HttpHeaders.ContentType, "application/json"))
            }
        }
        val api = JobsApi(client, "https://example.test")

        val sync = assertIs<CreateArchiveResponse.Status200>(api.createArchive(sync = true))
        assertContentEquals("PKzip".encodeToByteArray(), sync.body)

        val async = assertIs<CreateArchiveResponse.Status202>(api.createArchive())
        assertEquals(Job(id = "job-1"), async.body)
    }

    @Test
    fun `bodyless variants and undeclared statuses`() = runTest {
        var status = HttpStatusCode.NoContent
        val client = buildTestClient { _ -> respond("", status) }
        val api = JobsApi(client, "https://example.test")

        assertSame(RebuildResponse.Status204, api.rebuild())
        status = HttpStatusCode.OK
        assertSame(PingResponse.Status200, api.ping())
        status = HttpStatusCode.Created
        assertFailsWith<ResponseException> { api.ping() }
        status = HttpStatusCode.InternalServerError
        assertFailsWith<ResponseException> { api.ping() }
    }

    @Test
    fun `a wildcard request media type is sent as application_octet-stream`() = runTest {
        var captured: HttpRequestData? = null
        val client = buildTestClient { request ->
            captured = request
            respond("x".encodeToByteArray(), HttpStatusCode.OK)
        }
        FilesApi(client, "https://example.test").echoFile(fileId = "x", body = "x".encodeToByteArray())
        assertEquals("application/octet-stream", captured!!.body.contentType?.toString())
    }
}
