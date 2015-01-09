#include "lasote/docker_client/client.h"
#include "lasote/simple_logger/logger.h"
#include <iostream>

using namespace lasote;

int main() {

  DockerClient client("http://localhost:4243");

  ERR_F error_cb = [] (int status, string desc) {
	  cout << "Error: " << status <<  endl  << desc;
  };


  //Command parameters
  jsonxx::Array commands;
  commands << "/bin/python";

  jsonxx::Object create_params;
  
  create_params << "Cmd" << commands;
  create_params << "Image" << "fedora_lint";

   auto c2 = client.create_container([&client, error_cb] ( jsonxx::Object ret) {
     cout << "CREATE CONTAINER RETURN: " << ret.json() << endl;
     auto container_id = ret.get<jsonxx::String>("Id");
     cout << "CONTAINER ID: " << container_id << endl;
     auto c4 = client.start_container([container_id, &client, error_cb] ( jsonxx::Object ret) {
        cout << "START CONTAINER RETURN: " << ret.json() << endl;
        auto c2 = client.attach_to_container(
            [] ( jsonxx::Object ret) {
               cout << "Response: " << ret.json() << endl;
            },
            error_cb,
            [] (const char *p, size_t len) {
                cout << string(p, len) ;
            },
            container_id, true, true, true, true);
     }, error_cb, container_id);
  }, error_cb, create_params);



  auto c5 = client.logs_container([] (string out) {
       cout << "Response: " << out << endl;
  }, error_cb, "c7ddced66641", true, true, true, true, "all");


  auto c6 = client.list_containers([] ( jsonxx::Object ret) {
    cout << "Containers: " << ret.json() << endl;
  }, error_cb, false, 13);

  auto c7 = client.list_images([] ( jsonxx::Object ret) {
      cout << "Images: " << ret.json() << endl;
  }, error_cb); 

  run_loop();
}
