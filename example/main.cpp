#include "lasote/docker_client/client.h"
#include "lasote/simple_logger/logger.h"
#include <iostream>

using namespace lasote;

int main() {

  DockerClient client("http://localhost:4243");

  ERR_F error_cb = [] (int status, string desc) {
	  cout << "Error: " << status <<  endl  << desc;
  };


  /*
   *
  Json create_params = Json::object{
	  {"Cmd", Json::array{"/bin/python"}},
	  {"Image", "fedora_lint"}
  };
   *
   * auto c2 = client.create_container([&client, error_cb] (Json ret) {
     string container_id = ret["Id"].string_value();
     auto c4 = client.start_container([container_id, &client, error_cb] (Json ret) {
        cout << "INFO: " << ret.dump() << endl;
        auto c2 = client.attach_to_container(
            [] (Json ret) {
               cout << "Response: " << ret.dump() << endl;
            },
            error_cb,
            [] (const char *p, size_t len) {
                cout << string(p, len) ;
            },
            container_id, true, true, true, true);
     }, error_cb, container_id);
  }, error_cb, create_params); */



  /*auto c2 = client.attach_to_container(
    [] (Json ret) {
       cout << "Response: " << ret.dump() << endl;
    },
    error_cb,
    [] (const char *p, size_t len) {
        cout << string(p, len) ;
    },
    "5c0e984c4cec", true, true, true, true);*/

  auto c3 = client.logs_container([] (string out) {
       cout << "Response: " << out << endl;
  }, error_cb, "c7ddced66641", true, true, true, true, "all");


 /* auto c3 = client.list_containers([] (Json ret) {
    cout << "Containers: " << ret.dump() << endl;
  }, error_cb, false, 13);

  auto c4 = client.list_images([] (Json ret) {
      cout << "Images: " << ret.dump() << endl;
  }, error_cb); */

  run_loop();
}
