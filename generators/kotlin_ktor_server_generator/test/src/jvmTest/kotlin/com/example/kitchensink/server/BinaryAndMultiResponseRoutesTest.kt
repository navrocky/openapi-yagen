package com.example.kitchensink.server

import com.example.kitchensink.server.fakes.FakeFilesApiHandler
import com.example.kitchensink.server.models.Job
import com.example.kitchensink.server.support.installKitchenSinkApp
import io.ktor.client.request.header
import io.ktor.client.request.get
import io.ktor.client.request.post
import io.ktor.client.request.put
import io.ktor.client.request.setBody
import io.ktor.client.statement.bodyAsText
import io.ktor.http.ContentType
import io.ktor.http.HttpHeaders
import io.ktor.http.HttpStatusCode
import io.ktor.http.contentType
import io.ktor.http.toHttpDate
import io.ktor.server.testing.testApplication
import kotlinx.serialization.json.Json
import kotlin.test.Test
import kotlin.test.assertEquals

class BinaryAndMultiResponseRoutesTest {
    @Test
    fun `putFile streams the request body`() = testApplication {
        val files = FakeFilesApiHandler()
        installKitchenSinkApp(filesHandler = files)
        val response = client.put("/files/a") {
            contentType(ContentType.Application.OctetStream)
            setBody("0123456789")
        }
        assertEquals(HttpStatusCode.NoContent, response.status)
        assertEquals("0123456789", files.files["a"]?.decodeToString())
    }

    @Test
    fun `putFile rejects a mismatched Content-Type with 415`() = testApplication {
        installKitchenSinkApp()
        val response = client.put("/files/a") {
            contentType(ContentType.Application.Json)
            setBody("{}")
        }
        assertEquals(HttpStatusCode.UnsupportedMediaType, response.status)
    }

    @Test
    fun `putFile rejects a missing required body with 400`() = testApplication {
        installKitchenSinkApp()
        assertEquals(HttpStatusCode.BadRequest, client.put("/files/a").status)
    }

    @Test
    fun `echoFile accepts a wildcard media type, an absent optional body, and enforces bytes() limit`() = testApplication {
        val files = FakeFilesApiHandler()
        installKitchenSinkApp(filesHandler = files)
        val echoed = client.post("/files/x/echo") {
            contentType(ContentType.parse("application/pdf; charset=binary"))
            setBody("%PDF")
        }
        assertEquals(HttpStatusCode.OK, echoed.status)
        assertEquals("%PDF", echoed.bodyAsText())
        assertEquals("application/pdf", files.lastContentType)
        assertEquals(ContentType.parse("application/zip"), echoed.contentType())

        val wrongType = client.post("/files/x/echo") {
            contentType(ContentType.Text.Plain)
            setBody("hi")
        }
        assertEquals(HttpStatusCode.UnsupportedMediaType, wrongType.status)

        val tooLarge = client.post("/files/x/echo") {
            contentType(ContentType.Application.OctetStream)
            setBody("x".repeat(100))
        }
        assertEquals(HttpStatusCode.PayloadTooLarge, tooLarge.status)

        val empty = client.post("/files/x/echo")
        assertEquals("empty", empty.bodyAsText())
    }

    @Test
    fun `getFile streams with Content-Disposition, Range and conditional requests`() = testApplication {
        installKitchenSinkApp()
        val full = client.get("/files/hello")
        assertEquals(HttpStatusCode.OK, full.status)
        assertEquals("hello, world", full.bodyAsText())
        assertEquals(ContentType.Application.OctetStream, full.contentType())
        assertEquals("attachment; filename=hello.bin", full.headers[HttpHeaders.ContentDisposition])
        assertEquals(FakeFilesApiHandler.MOD_TIME.toHttpDate(), full.headers[HttpHeaders.LastModified])

        val partial = client.get("/files/hello") { header(HttpHeaders.Range, "bytes=7-11") }
        assertEquals(HttpStatusCode.PartialContent, partial.status)
        assertEquals("world", partial.bodyAsText())

        val notModified = client.get("/files/hello") {
            header(HttpHeaders.IfModifiedSince, FakeFilesApiHandler.MOD_TIME.toHttpDate())
        }
        assertEquals(HttpStatusCode.NotModified, notModified.status)
    }

    @Test
    fun `createArchive picks the status by the returned variant`() = testApplication {
        installKitchenSinkApp()
        val sync = client.post("/jobs/archive?sync=true")
        assertEquals(HttpStatusCode.OK, sync.status)
        assertEquals("PKzip", sync.bodyAsText())
        assertEquals(ContentType.parse("application/zip"), sync.contentType())
        assertEquals("attachment; filename=archive.zip", sync.headers[HttpHeaders.ContentDisposition])

        val async = client.post("/jobs/archive")
        assertEquals(HttpStatusCode.Accepted, async.status)
        assertEquals(Job(id = "job-1"), Json.decodeFromString<Job>(async.bodyAsText()))

        assertEquals(HttpStatusCode.NoContent, client.post("/jobs/rebuild").status)
        assertEquals(HttpStatusCode.OK, client.post("/jobs/ping").status)
    }
}
