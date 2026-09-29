package client

import (
	"fmt"
	"io"
	"mime"
	"mime/multipart"
	"net/http"
	"net/url"
	"strings"
	"time"
)

// formatParam converts a path/query/header parameter value to its wire string representation.
// time.Time is formatted as RFC3339 (fmt.Sprint's default time.Time formatting is not wire-safe);
// every other supported parameter type prints its natural wire value directly, including a
// generated enum type (its underlying string/int representation IS its wire value).
func formatParam(v any) string {
	if t, ok := v.(time.Time); ok {
		return t.Format(time.RFC3339)
	}
	return fmt.Sprint(v)
}

func pathParam(v any) string {
	return url.PathEscape(formatParam(v))
}

func addQueryParam(q url.Values, name string, v any) {
	q.Set(name, formatParam(v))
}

// OpenAPI 3's default array-typed query parameter serialization (`style: form, explode: true`):
// one repeated `name=` key per element, e.g. `?ids=1&ids=2` - not a single comma-joined value.
func addQueryParamList[T any](q url.Values, name string, values []T) {
	for _, v := range values {
		q.Add(name, formatParam(v))
	}
}

// Comma/space/pipe-joined single-value array query parameter serialization (OpenAPI's
// `explode: false`, style: form/spaceDelimited/pipeDelimited) - the counterpart to
// addQueryParamList's repeated-key form (explode: true).
func addQueryParamJoined[T any](q url.Values, name string, values []T, sep string) {
	if len(values) == 0 {
		return
	}
	parts := make([]string, len(values))
	for i, v := range values {
		parts[i] = formatParam(v)
	}
	q.Set(name, strings.Join(parts, sep))
}

// writeMultipartFile writes a multipart `format: binary` field as an actual file part (via
// CreateFormFile) rather than a text field - name doubles as both the form field name and the
// part's filename, since the generated model only carries the file's bytes, not a separate
// filename. A nil data (an absent optional file field - []byte is never pointer-wrapped, see
// types.js's isRefType) writes nothing.
func writeMultipartFile(w *multipart.Writer, name string, data []byte) error {
	if data == nil {
		return nil
	}
	part, err := w.CreateFormFile(name, name)
	if err != nil {
		return err
	}
	_, err = part.Write(data)
	return err
}

// ResponseError is returned when a request completes but the server responded with an
// unsuccessful (4xx/5xx) status code.
type ResponseError struct {
	StatusCode int
	Body       []byte
}

func (e *ResponseError) Error() string {
	return fmt.Sprintf("unexpected status %d: %s", e.StatusCode, string(e.Body))
}

// File is a raw (non-JSON, non-text) response body, still unread: stream it via Body, or call
// Bytes() to read it whole. Either way the caller must consume it - Bytes() closes Body itself,
// otherwise call Close() when done.
type File struct {
	// Body is the response body, read directly from the connection.
	Body io.ReadCloser
	// ContentType is the response's Content-Type header.
	ContentType string
	// ContentLength is the response's declared body length, or -1 if unknown.
	ContentLength int64
	// Name is the filename from a Content-Disposition response header, or "" if there is none.
	Name string
	// StatusCode is the actual response status (e.g. 206 for a partial Range response).
	StatusCode int
	// Header is the full set of response headers.
	Header http.Header
}

// Bytes reads the whole remaining body into memory and closes it.
func (f *File) Bytes() ([]byte, error) {
	defer f.Body.Close()
	return io.ReadAll(f.Body)
}

// Close closes the body without reading the rest of it.
func (f *File) Close() error { return f.Body.Close() }

func newFile(resp *http.Response) *File {
	f := &File{
		Body:          resp.Body,
		ContentType:   resp.Header.Get("Content-Type"),
		ContentLength: resp.ContentLength,
		StatusCode:    resp.StatusCode,
		Header:        resp.Header,
	}
	if disposition := resp.Header.Get("Content-Disposition"); disposition != "" {
		if _, params, err := mime.ParseMediaType(disposition); err == nil {
			f.Name = params["filename"]
		}
	}
	return f
}
