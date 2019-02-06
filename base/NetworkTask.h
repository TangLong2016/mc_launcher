#pragma once

#include "curl_easy.h"
#include "sigslot.h"

#include <memory>

class NetworkTask
	: public std::enable_shared_from_this<NetworkTask>
{
	friend class NetworkTaskDispatcher;

public:
	NetworkTask();
	virtual ~NetworkTask();

	enum State
	{
		kStop = 0,
		kWaiting,
		kRunning,
	};
public:
	curl::curl_easy& easy()
	{
		return easy_;
	}

	State state() { return state_; }

	virtual uint64_t dl_current() { return dl_curr_; }
	virtual uint64_t dl_total() { return dl_total_; }

	void set_cancel(bool cancel) { cancel_ = cancel; }
	bool is_cancelled() { return cancel_; }
	uint32_t task_id() { return task_id_; }

	sigslot::signal2<std::shared_ptr<NetworkTask>, State> state_changed;

protected:
	static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static size_t header_callback(void *buffer, size_t size, size_t nitems, void *userdata);
	void set_state(State s) { state_ = s; }

	void set_curl_result(CURLcode res) { response_code_ = res; }
	CURLcode curl_result() { return response_code_; }

	std::string& response_header() { return response_header_; }

	std::string last_modified();

	//�ڱ����ص���������֮ǰ���á�����ڱ�����֮ǰcancel�ˣ��򲻻�����ûص�
	//����ֵ������false�Ļ��������Ͳ��ᱻ�����ˣ����������Ҳ�������on_after_schedule�ص�
	virtual bool on_before_schedule();

	//�����������Ѿ������ȣ���ô�ڵ������֮�󣨼�ʹ������cancel����������ûص�
	virtual bool on_after_schedule();

	static uint32_t generate_task_id();

	virtual struct curl_slist *on_set_header(struct curl_slist* h);
private:
	curl::curl_easy easy_;
	bool cancel_ = false;
	State state_ = kStop;
	double dl_curr_ = 0;
	double dl_total_ = 0;
	struct curl_slist *headers_ = nullptr;

	CURLcode response_code_ = CURLE_OK;

	std::string response_header_;
	uint32_t task_id_ = NetworkTask::generate_task_id();
};

