#include "lasote/json11/json11.hpp"
#include "client.h"
#include <memory>

DockerClient::DockerClient(string host) : uri(host){
}


shared_ptr<LambdaRequest> DockerClient::system_info(JSON_F ret_cb, ERR_F err_cb){
	string uri = "/info";
    return get_and_parse_response(uri, ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::docker_version(JSON_F ret_cb, ERR_F err_cb){
	string uri = "/version";
	return get_and_parse_response(uri, ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::list_containers(JSON_F ret_cb, ERR_F err_cb, bool all, int limit, string since, string before, int size, Json filters){

	string uri = "/containers/json?";
	uri += param("all", all);
	uri += param("limit", limit);
	uri += param("since", since);
	uri += param("before", before);
	uri += param("size", size);
	uri += param("filters", filters);

	return get_and_parse_response(uri, ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::inspect_container(JSON_F ret_cb, ERR_F err_cb, string container_id){
	string uri = "/containers/" + container_id + "/json";
	return get_and_parse_response(uri, ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::top_container(JSON_F ret_cb, ERR_F err_cb, string container_id){
	string uri = "/containers/" + container_id + "/top";
	return get_and_parse_response(uri, ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::logs_container(STRING_F ret_cb, ERR_F err_cb, string container_id, bool follow, bool stdout, bool stderr, bool timestamps, string tail){
	string uri = "/containers/" + container_id + "/logs?";
	uri += param("follow", follow);
	uri += param("stdout", stdout);
	uri += param("stderr", stderr);
	uri += param("timestamps", timestamps);
	uri += param("tail", tail);
	return get_and_parse_response(uri, ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::get_container_changes(JSON_F ret_cb, ERR_F err_cb, string container_id){
	string uri = "/containers/" + container_id + "/changes";
	return get_and_parse_response(uri, ret_cb, err_cb);
}

shared_ptr<LambdaRequest>  DockerClient::create_container(JSON_F ret_cb, ERR_F err_cb, Json parameters){
	string uri = "/containers/create";
	//string tmp = "{\"Hostname\":\"\",\"Domainname\": \"\",\"User\":\"\",\"Memory\":0,\"MemorySwap\":0,\"CpuShares\": 512,\"Cpuset\": \"0,1\",\"AttachStdin\":false,\"AttachStdout\":true,\"AttachStderr\":true,\"PortSpecs\":null,\"Tty\":false,\"OpenStdin\":false,\"StdinOnce\":false,\"Env\":null,\"Cmd\":[ \"date\"],\"Image\":\"base\",\"Volumes\":{ \"/tmp\": {}},\"WorkingDir\":\"\",\"NetworkDisabled\": false,\"ExposedPorts\":{ \"22/tcp\": {}}}";
	return post_and_parse_json_response(uri, parameters.dump(), ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::start_container(JSON_F ret_cb, ERR_F err_cb, string container_id){
	string uri = "/containers/" + container_id + "/start";
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::stop_container(JSON_F ret_cb, ERR_F err_cb, string container_id, int delay){
	string uri = "/containers/" + container_id + "/stop?";
	uri += param("t", delay);
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::kill_container(JSON_F ret_cb, ERR_F err_cb, string container_id, int signal){
	string uri = "/containers/" + container_id + "/kill?";
	uri += param("signal", signal);
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::pause_container(JSON_F ret_cb, ERR_F err_cb, string container_id){
	string uri = "/containers/" + container_id + "/pause";
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::unpause_container(JSON_F ret_cb, ERR_F err_cb, string container_id){
	string uri = "/containers/" + container_id + "/unpause?";
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::wait_container(JSON_F ret_cb, ERR_F err_cb, string container_id){
	string uri = "/containers/" + container_id + "/wait";
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::delete_container(JSON_F ret_cb, ERR_F err_cb, string container_id, bool v, bool force){
	string uri = "/containers/" + container_id + "/delete?";
	uri += param("v", v);
	uri += param("force", force);
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}

shared_ptr<LambdaRequest> DockerClient::copy_from_container(STRING_F ret_cb, ERR_F err_cb, string container_id, string file_path, string dest_tar_file){
	string path = "/containers/" + container_id + "/copy";
	Json body = Json::object{
		  {"Resource", file_path}
	};
	string tmp = body.dump();
	Request request;
	Method method("POST", uri + path);
	request.method = &method;
	request.body = tmp;
	std::pair<string,string> content_type("Content-Type", "application/json");
    request.headers.insert(content_type);
    std::pair<string,string> content_len("Content-Length", to_string(tmp.length()));
    request.headers.insert(content_len);

    auto request_call = std::make_shared<LambdaRequest>();

	request_call->on_message_complete_cb = [request_call, ret_cb, err_cb, dest_tar_file] (int status) {
		if(status > 299){
			debug("Error calling: " << status);
			if(err_cb != NULL){
				err_cb(status, request_call->response_buffer);
			}
		}
		else{
			debug("Status: " << status);
			//Write file to dest_tar_file
			ofstream myfile;
		    myfile.open(dest_tar_file);
		    myfile << request_call->response_buffer;
		    myfile.close();
			ret_cb(dest_tar_file);
		}
	};
	request_call->send(request);
	return request_call;

}

shared_ptr<LambdaRequest> DockerClient::attach_to_container (JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb, string container_id, bool logs, bool stream, bool stdin, bool stdout, bool stderr){
	string uri = "/containers/" + container_id + "/attach?";
	uri += param("logs", logs);
	uri += param("stream", stream);
	uri += param("stdin", stdin);
	uri += param("stdout", stdout);
	uri += param("stderr", stderr);
	return post_and_parse_json_response(uri, "", ret_cb, err_cb, on_body_cb);
}


shared_ptr<LambdaRequest> DockerClient::restart_container(JSON_F ret_cb, ERR_F err_cb, string container_id, int delay){
	string uri = "/containers/" + container_id + "/restart?";
	uri += param("t", delay);
	return post_and_parse_json_response(uri, "", ret_cb, err_cb);
}


shared_ptr<LambdaRequest> DockerClient::list_images(JSON_F ret_cb, ERR_F err_cb){
	string uri = "/images/json";
	return get_and_parse_response(uri, ret_cb, err_cb);
}


shared_ptr<LambdaRequest> DockerClient::get_and_parse_response(string path, JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb){
	info(path);
	Request request;
	Method method("GET", uri + path);
	request.method = &method;
	return call_and_parse_response(request, ret_cb, err_cb, on_body_cb);
}

shared_ptr<LambdaRequest> DockerClient::get_and_parse_response(string path, STRING_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb){
	info(path);
	Request request;
	Method method("GET", uri + path);
	request.method = &method;
	return call_and_parse_response(request, ret_cb, err_cb, on_body_cb);
}

shared_ptr<LambdaRequest> DockerClient::post_and_parse_json_response(string path, string body, JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb){
	info(path);
	Request request;
	Method method("POST", uri + path);
	request.method = &method;
	request.body = body;
	std::pair<string,string> content_type("Content-Type", "application/json");
    request.headers.insert(content_type);
    std::pair<string,string> content_len("Content-Length", to_string(body.length()));
    request.headers.insert(content_len);

    return call_and_parse_response(request, ret_cb, err_cb, on_body_cb);
}

shared_ptr<LambdaRequest> DockerClient::call_and_parse_response(Request& request, JSON_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb){
	auto request_call = std::make_shared<LambdaRequest>();

	request_call->on_message_complete_cb = [request_call, ret_cb, err_cb] (int status) {
		if(status > 299){
			debug("Error calling: " << status);
			if(err_cb != NULL){
				err_cb(status, request_call->response_buffer);
			}
		}
		else{
			debug("Status: " << status);
			string err;
			Json json = Json::parse(request_call->response_buffer, err);
			ret_cb(json);
		}
	};
	request_call->on_body_cb = on_body_cb;
	request_call->send(request);
	return request_call;
}

shared_ptr<LambdaRequest> DockerClient::call_and_parse_response(Request& request, STRING_F ret_cb, ERR_F err_cb, CHAR_PTR_F on_body_cb){
	auto request_call = std::make_shared<LambdaRequest>();

	request_call->on_message_complete_cb = [request_call, ret_cb, err_cb] (int status) {
		if(status > 299){
			debug("Error calling: " << status);
			if(err_cb != NULL){
				err_cb(status, request_call->response_buffer);
			}
		}
		else{
			debug("Status: " << status);
			ret_cb(request_call->response_buffer);
		}
	};
	request_call->on_body_cb = on_body_cb;
	request_call->send(request);
	return request_call;
}


void run_loop(){
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}


string param( string param_name, string param_value){
	if(!param_value.empty()){
		return "&" + param_name + "=" + param_value;
	}
	else{
		return "";
	}
}

string param( string param_name, const char* param_value){
	if(param_value != NULL){
		return "&" + param_name + "=" + param_value;
	}
	else{
		return "";
	}
}

string param(string param_name, bool param_value){
	string ret;
	ret = "&" + param_name + "=";
	if(param_value){
		return ret + "true";
	}
	else{
		return ret + "false";
	}
}

string param(string param_name, int param_value){
	if(param_value != -1){
		return "&" + param_name + "=" + std::to_string(param_value);
	}
	else{
		return "";
	}
}

string param( string param_name, Json param_value){
	if(!param_value.is_null()){
		return "&" + param_name + "=" + param_value.dump();
	}
	else{
		return "";
	}
}

