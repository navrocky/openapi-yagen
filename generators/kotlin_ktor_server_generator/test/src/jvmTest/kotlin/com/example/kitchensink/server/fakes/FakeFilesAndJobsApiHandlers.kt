package com.example.kitchensink.server.fakes

import com.example.kitchensink.server.BinaryBody
import com.example.kitchensink.server.BinaryContent
import com.example.kitchensink.server.apis.CreateArchiveResponse
import com.example.kitchensink.server.apis.FilesApiHandler
import com.example.kitchensink.server.apis.JobsApiHandler
import com.example.kitchensink.server.apis.PingResponse
import com.example.kitchensink.server.apis.RebuildResponse
import com.example.kitchensink.server.models.Job
import io.ktor.util.date.GMTDate
import io.ktor.utils.io.ByteReadChannel
import io.ktor.utils.io.toByteArray

// Only ever reads/writes raw bodies.
class FakeFilesApiHandler : FilesApiHandler {
    val files = mutableMapOf("hello" to "hello, world".encodeToByteArray())
    var lastContentType: String? = null

    override suspend fun getFile(fileId: String): BinaryContent {
        val data = files[fileId] ?: throw NotFoundException("file $fileId not found")
        // Streamed through a channel (not the in-memory constructor), with a known length so
        // Range requests still work.
        return BinaryContent(data.size.toLong(), name = "$fileId.bin", lastModified = MOD_TIME) { ByteReadChannel(data) }
    }

    override suspend fun putFile(fileId: String, body: BinaryBody) {
        // Streamed, not buffered via bytes() - exercises BinaryBody as a channel.
        files[fileId] = body.channel().toByteArray()
    }

    override suspend fun echoFile(fileId: String, body: BinaryBody?): BinaryContent {
        if (body == null) return BinaryContent("empty".encodeToByteArray())
        lastContentType = body.contentType.toString()
        return BinaryContent(body.bytes(maxBytes = 16))
    }

    companion object {
        val MOD_TIME = GMTDate(1_767_322_800_000)
    }
}

class FakeJobsApiHandler : JobsApiHandler {
    override suspend fun createArchive(sync: Boolean?): CreateArchiveResponse =
        if (sync == true) {
            CreateArchiveResponse.Status200(BinaryContent(null, name = "archive.zip") { ByteReadChannel("PKzip".encodeToByteArray()) })
        } else {
            CreateArchiveResponse.Status202(Job(id = "job-1"))
        }

    override suspend fun rebuild(): RebuildResponse = RebuildResponse.Status204

    override suspend fun ping(): PingResponse = PingResponse.Status200
}
