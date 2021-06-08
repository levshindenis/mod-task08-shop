#include <iostream>
#include <deque>
#include <thread>
#include <vector>
#include <future>


class Client
{
public:
    int truck_load_capacity;
    int id_client;
    Client(int id_client, int products_count);
};

class Shop
{
    struct Statistics
    {
        int work = 0;
        int plain = 0;
        int queue = 0;
        int wait = 0;
        int served = 0;
        int rejected = 0;
        int observations_count = 0;
    };


    class C
    {
        int cur_client_id = -1;
        std::future<void> thrd;   
        int process_time;
        void process(int truck_load_capacity);

    public:
        Statistics* stats;
        C(int process_time, Statistics& stats);
        bool in_use = false;   
        void start_processing(Client client);
    };

    void monitoring();
    std::deque<Client> client_queue;
    int D;
    std::vector<C> cashbox_pool;
    int monitoring_interval;  
    std::thread main_thread;  
    std::thread FLOW;
    bool F = false;

public:
    void Run();
    void Start();
    void Stop();
    void add_to_queue(Client client);
    Shop(int threads_count, int queue_size, int process_time);
    Statistics stats;  


};
