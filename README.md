# C++ Docker Client

[Docker Rest API v1.16](https://docs.docker.com/reference/api/docker_remote_api_v1.16/) implementation with C++11 using lambda functions for callbacks.

This library is hosted in **Biicode C++ dependency manager**.

Biicode block [lasote/docker_client](http://www.biicode.com/lasote/docker_client)

Also in [github repository https://github.com/lasote/docker_client](https://github.com/lasote/docker_client)

**Want to try it?**

The project has many dependencies, we recommend use biicode to handle them:

1. [Get started with biicode](http://docs.biicode.com/c++/gettingstarted.html)
2. #include "lasote/docker_client/client.h" in your source code file
3. > bii find # to download the required dependencies
4. > bii cpp:build # to build the project

Take a look to the example folder in repository: http://www.biicode.com/lasote/docker_client

**Examples**
```
DockerClient client("http://localhost:4243");

ERR_F error_cb = [] (int status, string desc) {
    cout << "Error: " << status <<  endl  << desc;
};

auto c3 = client.logs_container([] (string out) {
       cout << "Response: " << out << endl;
}, error_cb, "c7ddced66641", true, true, true, true, "all");

auto c3 = client.list_containers([] (Json ret) {
  cout << "Containers: " << ret.dump() << endl;
}, error_cb, false, 13);

auto c4 = client.list_images([] (Json ret) {
    cout << "Images: " << ret.dump() << endl;
}, error_cb);

run_loop();

```

Take a look to **client.h** file to get a list of all available methods.
