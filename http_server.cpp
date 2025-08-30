/*
 * sample http server
 * more detail see examples/httpd
 *
 */

#include "HttpServer.h"
#include "hthread.h"    // import hv_gettid
#include "hasync.h"     // import hv::async
#include "mpp_control.h"
#include "tools.h"

using namespace hv;

/*
 * #define TEST_HTTPS 1
 *
 * @build   ./configure --with-openssl && make clean && make
 *
 * @server  bin/http_server_test 1883
 *
 * @client  curl -v http://127.0.0.1:1883/ping
 *          curl -v https://127.0.0.1:8443/ping --insecure
 *          bin/curl -v http://127.0.0.1:1883/ping
 *          bin/curl -v https://127.0.0.1:8443/ping
 *
 */

int main(int argc, char **argv) {
    HV_MEMCHECK;

    int port = 0;
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    if (port == 0) port = 1883;

    HttpService router;

    /* Static file service */
    // curl -v http://ip:port/
    router.Static("/", "./html");

    /* Forward proxy service */
    router.EnableForwardProxy();
    // curl -v http://httpbin.org/get --proxy http://127.0.0.1:1883
    router.AddTrustProxy("*httpbin.org");

    /* Reverse proxy service */
    // curl -v http://ip:port/httpbin/get
    router.Proxy("/httpbin/", "http://httpbin.org/");

    /* API handlers */
    // curl -v http://ip:port/ping
    router.GET("/ping", [](HttpRequest *req, HttpResponse *resp) {
        return resp->String("pong");
    });

    // curl -v http://ip:port/data
    router.GET("/data", [](HttpRequest *req, HttpResponse *resp) {
        static char data[] = "0123456789";
        return resp->Data(data, 10 /*, false */);
    });

    // curl -v http://ip:port/paths
    router.GET("/paths", [&router](HttpRequest *req, HttpResponse *resp) {
        return resp->Json(router.Paths());
    });

    // curl -v http://ip:port/get?env=1
    router.GET("/get", [](const HttpContextPtr &ctx) {
        hv::Json resp;
        resp["origin"] = ctx->ip();
        resp["url"] = ctx->url();
        resp["args"] = ctx->params();
        resp["headers"] = ctx->headers();
        return ctx->send(resp.dump(2));
    });

    // curl -v http://ip:port/echo -d "hello,world!"
    router.POST("/echo", [](const HttpContextPtr &ctx) {
        return ctx->send(ctx->body(), ctx->type());
    });

    // curl -v http://ip:port/user/123
    router.GET("/user/{id}", [](const HttpContextPtr &ctx) {
        hv::Json resp;
        resp["id"] = ctx->param("id");
        return ctx->send(resp.dump(2));
    });

    // curl -v http://ip:port/async
    router.GET("/async", [](const HttpRequestPtr &req, const HttpResponseWriterPtr &writer) {
        writer->Begin();
        writer->WriteHeader("X-Response-tid", hv_gettid());
        writer->WriteHeader("Content-Type", "text/plain");
        writer->WriteBody("This is an async response.\n");
        writer->End();
    });

    std::time_t last_time = 0;
    router.Any("/hook/on_server_started", [&last_time](const HttpContextPtr &ctx) {
        hv::Json resp;
        resp["origin"] = ctx->ip();
        resp["url"] = ctx->url();
        resp["args"] = ctx->params();
        resp["headers"] = ctx->headers();

        //on_server_started事件会执行2次
        std::time_t now_time = std::time(nullptr);
        std::cout << "last_time " << last_time << ",now_time " << now_time << std::endl;
        if (now_time - last_time >= 5) {
            last_time = now_time;
            mpp_control::restartProc(MEDIA_CLIENT_PATH);
        }

        return ctx->send(resp.dump(2));
    });

    router.Any("/hook/on_server_exited", [](const HttpContextPtr &ctx) {
        hv::Json resp;
        resp["origin"] = ctx->ip();
        resp["url"] = ctx->url();
        resp["args"] = ctx->params();
        resp["headers"] = ctx->headers();

        mpp_control::stopProc(MEDIA_CLIENT_PATH);
        return ctx->send(resp.dump(2));
    });
    mpp_control::init();

    // middleware
    router.AllowCORS();
    router.Use([](HttpRequest *req, HttpResponse *resp) {
        resp->SetHeader("X-Request-tid", hv::to_string(hv_gettid()));
        return HTTP_STATUS_NEXT;
    });

    HttpServer server;
    server.service = &router;
    server.port = port;

    // uncomment to test multi-processes
    // server.setProcessNum(4);
    // uncomment to test multi-threads
    // server.setThreadNum(4);

    server.run();

    return 0;
}
