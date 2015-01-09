#include "hjiang/jsonxx/jsonxx.h"
#include "lasote/lambda_http_client/http_request.h"
#include <string.h>
#include <iostream>
#include <functional>
#include <fstream>
#include <memory>

using namespace httpmodels;
using namespace lasote;

#define JSON_OBJECT jsonxx::Object
#define JSON_ARRAY jsonxx::Array
#define JSON_F function<void(JSON_OBJECT)>
#define CHAR_PTR_F function<void(const char *p, size_t len)>
#define STRING_F function<void(string ret)>
#define ERR_F function<void(int status, string desc)>


string param( string param_name, string param_value);
string param( string param_name, const char* param_value);
string param(string param_name, bool param_value);
string param(string param_name, int param_value);
string param( string param_name, JSON_OBJECT param_value);

void run_loop();

class DockerClient{
	public:
		DockerClient(string host);

		// System
		shared_ptr<LambdaRequest> system_info(JSON_F ret_cb, ERR_F err_cb);
		shared_ptr<LambdaRequest> docker_version(JSON_F ret_cb, ERR_F err_cb);


		// Images
		shared_ptr<LambdaRequest> list_images(JSON_F ret_cb, ERR_F err_cb);

		// Containers
		shared_ptr<LambdaRequest> list_containers(JSON_F ret_cb, ERR_F err_cb, bool all=false, int limit=-1, string since="", string before="", int size=-1, JSON_OBJECT filters=JSON_OBJECT());
		shared_ptr<LambdaRequest> inspect_container(JSON_F ret_cb, ERR_F err_cb, string container_id);
		shared_ptr<LambdaRequest> top_container(JSON_F ret_cb, ERR_F err_cb, string container_id);
		shared_ptr<LambdaRequest> logs_container(STRING_F ret_cb, ERR_F err_cb, string container_id, bool follow=false, bool o_stdout=true, bool o_stderr=false, bool timestamps=false, string tail="all");
		shared_ptr<LambdaRequest> create_container(JSON_F ret_cb, ERR_F err_cb, JSON_OBJECT parameters);
		shared_ptr<LambdaRequest> start_container(JSON_F ret_cb, ERR_F err_cb, string container_id);
		shared_ptr<LambdaRequest> get_container_changes(JSON_F ret_cb, ERR_F err_cb, string container_id);
		shared_ptr<LambdaRequest> stop_container(JSON_F ret_cb, ERR_F err_cb, string container_id, int delay=-1);
		shared_ptr<LambdaRequest> kill_container(JSON_F ret_cb, ERR_F err_cb, string container_id, int signal=-1);
		shared_ptr<LambdaRequest> pause_container(JSON_F ret_cb, ERR_F err_cb, string container_id);
		shared_ptr<LambdaRequest> wait_container(JSON_F ret_cb, ERR_F err_cb, string container_id);
		shared_ptr<LambdaRequest> delete_container(JSON_F ret_cb, ERR_F err_cb, string container_id, bool v=false, bool force=false);
		shared_ptr<LambdaRequest> unpause_container(JSON_F ret_cb, ERR_F err_cb, string container_id);
		shared_ptr<LambdaRequest> restart_container(JSON_F ret_cb, ERR_F err_cb, string container_id, int delay=-1);
		shared_ptr<LambdaRequest> attach_to_container(JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb, string container_id, bool logs=false, bool stream=false, bool o_stdin=false, bool o_stdout=false, bool o_stderr=false);
		shared_ptr<LambdaRequest> copy_from_container(STRING_F ret_cb, ERR_F err_cb, string container_id, string file_path, string dest_tar_file);

		string uri;
	private:
		shared_ptr<LambdaRequest> call_and_parse_response(Request& request, JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb=CHAR_PTR_F());
		shared_ptr<LambdaRequest> call_and_parse_response(Request& request, STRING_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb=CHAR_PTR_F());
		shared_ptr<LambdaRequest> get_and_parse_response(string path, JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb=CHAR_PTR_F());
		shared_ptr<LambdaRequest> get_and_parse_response(string path, STRING_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb=CHAR_PTR_F());
		shared_ptr<LambdaRequest> post_and_parse_json_response(string path, string body, JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb=CHAR_PTR_F());
};

