import { test } from "node:test";
import assert from "node:assert/strict";
import { ApiError } from "../generated/runtime.js";
import { FilesClient } from "../generated/apis/FilesClient.js";
import { JobsClient } from "../generated/apis/JobsClient.js";
import type { CreateArchiveResponse } from "../generated/index.js";
import { createFetchStub } from "./support/fetchStub.js";

test("createArchive resolves to the variant matching the response status", async () => {
  const { fetch } = createFetchStub((req) =>
    req.url.includes("sync=true")
      ? { status: 200, body: new TextEncoder().encode("PKzip"), headers: { "content-type": "application/zip" } }
      : { status: 202, body: { id: "job-1" }, headers: { "content-type": "application/json" } }
  );
  const client = new JobsClient({ baseUrl: "https://example.test", fetch });

  const sync: CreateArchiveResponse = await client.createArchive({ sync: true });
  assert.equal(sync.status, 200);
  if (sync.status === 200) assert.equal(new TextDecoder().decode(sync.body), "PKzip");

  const accepted = await client.createArchive();
  assert.equal(accepted.status, 202);
  if (accepted.status === 202) assert.equal(accepted.body.id, "job-1");
});

test("bodyless variants resolve to just their status; undeclared statuses throw ApiError", async () => {
  let status = 204;
  const { fetch } = createFetchStub(() => ({ status }));
  const client = new JobsClient({ baseUrl: "https://example.test", fetch });

  assert.deepEqual(await client.rebuild(), { status: 204 });
  status = 200;
  assert.deepEqual(await client.ping(), { status: 200 });
  status = 201;
  await assert.rejects(client.ping(), (e: unknown) => e instanceof ApiError && e.status === 201);
  status = 500;
  await assert.rejects(client.ping(), (e: unknown) => e instanceof ApiError && e.status === 500);
});

test("a wildcard request media type is sent as application/octet-stream", async () => {
  const { fetch, calls } = createFetchStub(() => ({ status: 200, body: new Uint8Array([1]) }));
  await new FilesClient({ baseUrl: "https://example.test", fetch }).echoFile("x", { body: new Uint8Array([1]) });
  assert.equal(calls[0]!.headers["content-type"], "application/octet-stream");
});
