#pragma once

#include "SessionInterface.h"
#include "ModuleInterfalce.h"
#include "serverconfigtype.h"
#include "ICommunicationService.h"
#include "ISessionService.h"
#include "IotoIo.h"
#include "CoreTimer.hpp"

//�����̵߳��߼��������ģ��
//add by freeeyes

//�����̵߳�ִ��״̬
enum class ENUM_WORK_THREAD_STATE
{
    WORK_THREAD_INIT = 0,
    WORK_THREAD_BEGIN,
    WORK_THREAD_END,
};

class CDelayPluginMessage
{
public:
    uint16 tag_thread_id_ = 0;
    std::string message_tag_ = "";
    CMessage_Packet send_packet_;
    CFrame_Message_Delay delay_timer_;
};

class CModuleLogic
{
public:
    CModuleLogic() = default;

    void init_logic(command_to_module_function command_to_module_function, uint16 work_thread_id);

    void add_session(uint32 connect_id, shared_ptr<ISession> session, const _ClientIPInfo& local_info, const _ClientIPInfo& romote_info);

    shared_ptr<ISession> get_session_interface(uint32 connect_id);

    void delete_session_interface(uint32 connect_id);

    void close();

    int do_thread_module_logic(const CMessage_Source& source, const CMessage_Packet& recv_packet, CMessage_Packet& send_packet);

    uint16 get_work_thread_id();

    int get_work_thread_timeout();

    void check_session_io_timeout(uint32 connect_timeout);

private:
    CSessionInterface sessions_interface_;
    CModuleInterface modules_interface_;
    uint16 work_thread_id_ = 0;

    ENUM_WORK_THREAD_STATE work_thread_state_ = ENUM_WORK_THREAD_STATE::WORK_THREAD_INIT;
    std::chrono::steady_clock::time_point work_thread_run_time_ = std::chrono::steady_clock::now();
};

class CWorkThreadLogic 
{
public:
    CWorkThreadLogic() = default;

    void init_work_thread_logic(int thread_count, uint16 timeout_seconds, uint32 connect_timeout, config_logic_list& logic_list, ISessionService* session_service);

    void init_communication_service(ICommunicationInterface* communicate_service);

    void close();

    void add_frame_events(uint16 command_id, uint32 mark_id, std::string remote_ip, uint16 remote_port, EM_CONNECT_IO_TYPE io_type);

    void add_thread_session(uint32 connect_id, shared_ptr<ISession> session, _ClientIPInfo& local_info, const _ClientIPInfo& romote_info);

    void delete_thread_session(uint32 connect_id, _ClientIPInfo from_io, shared_ptr<ISession> session);

    void close_session_event(uint32 connect_id);

    int do_thread_module_logic(const uint32 connect_id, vector<CMessage_Packet>& message_list, shared_ptr<ISession> session);

    void send_io_message(uint32 connect_id, CMessage_Packet send_packet);

    bool connect_io_server(const CConnect_IO_Info& io_info, EM_CONNECT_IO_TYPE io_type);

    void close_io_server(uint32 server_id);

    uint32 get_io_server_id(uint32 connect_id);

    bool add_session_io_mapping(_ClientIPInfo from_io, EM_CONNECT_IO_TYPE from_io_type, _ClientIPInfo to_io, EM_CONNECT_IO_TYPE to_io_type);

    bool delete_session_io_mapping(_ClientIPInfo from_io, EM_CONNECT_IO_TYPE from_io_type);

    void run_check_task(uint32 timeout_seconds);

    bool send_frame_message(uint16 tag_thread_id, std::string message_tag, CMessage_Packet send_packet, CFrame_Message_Delay delay_timer);

    void do_plugin_thread_module_logic(shared_ptr<CModuleLogic> module_logic, std::string message_tag, CMessage_Packet recv_packet);

    bool create_frame_work_thread(uint32 thread_id);

    bool close_frame_work_thread(uint32 thread_id);

    bool delete_frame_message_timer(uint64 timer_id);

    uint16 get_io_work_thread_count();

    uint16 get_plugin_work_thread_count();

private:
    using hashmappluginworkthread = unordered_map<uint32, shared_ptr<CModuleLogic>>;
    using hashmaplogictimer = unordered_map<uint64, brynet::Timer::WeakPtr>;
    vector<shared_ptr<CModuleLogic>> thread_module_list_;
    CLoadModule load_module_;
    uint16      thread_count_ = 0;
    CIotoIo     io_to_io_;
    ICommunicationInterface* communicate_service_ = nullptr;
    bool        module_init_finish_ = false;
    vector<uint32> plugin_work_thread_buffer_list_;
    vector<CDelayPluginMessage> plugin_work_thread_buffer_message_list_;
    hashmappluginworkthread plugin_work_thread_list_;
    hashmaplogictimer plgin_timer_list_;
    std::recursive_mutex plugin_timer_mutex_;
    uint32 connect_timeout_ = 0;
};

using App_WorkThreadLogic = PSS_singleton<CWorkThreadLogic>;
