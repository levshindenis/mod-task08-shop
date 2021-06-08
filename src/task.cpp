#include "task.h"
#include <iostream>
#include <iostream>
#include <deque>
#include <thread>
#include <vector>
#include <future>


Shop::Shop(int threads_count, int queue_size, int process_time) 
{
    cashbox_pool = std::vector<C>();
    for (int i = 0; i < threads_count; i++) 
    {
        cashbox_pool.push_back(C(process_time, stats));
    }

    stats.plain = 0;
    stats.work = 0;
    stats.wait = 0;

    client_queue = std::deque<Client>();
    D = queue_size;
}



void Shop::monitoring() 
{
    while (F) 
    {
        for (int i = 0; i < cashbox_pool.size(); i++) 
        {
            if (cashbox_pool[i].in_use) 
            {
                stats.work += monitoring_interval;
            }
            else 
            {
                stats.plain += monitoring_interval;
            }
        }
        stats.wait += client_queue.size() * monitoring_interval;
        stats.queue += client_queue.size();
        stats.observations_count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(monitoring_interval));
    }
}

void Shop::Run() 
{
    while (F)
    {
        for (int i = 0; i < cashbox_pool.size(); i++) 
        {
            if (!cashbox_pool[i].in_use && client_queue.size() > 0) 
            {
                cashbox_pool[i].start_processing(client_queue.front());
                client_queue.pop_front();
            }
        }
    }
}

void Shop::Stop() 
{
    F = false;
    stats.rejected += client_queue.size();
    FLOW.join();
    main_thread.join();
}

void Shop::Start() 
{
    F = true;
    FLOW = std::thread(&Shop::monitoring, this);
    main_thread = std::thread(&Shop::Run, this);
}

void Shop::add_to_queue(Client client) 
{
    if (D > client_queue.size()) 
    {
        client_queue.push_back(client);
    }
    else
    {
        stats.rejected++;
    }
}

Shop::C::C(int process_time, Statistics& stats) 
{
    this->stats = &stats;
    in_use = false;
    this->process_time = process_time;
}

void Shop::C::start_processing(Client client) 
{
    this->in_use = true;
    cur_client_id = client.id_client;
    thrd = std::async(&Shop::C::process, this, client.truck_load_capacity);
}

void Shop::C::process(int truck_load_capacity) 
{
    for (int i = 0; i < truck_load_capacity; i++)   
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(this->process_time));
    }
    //ToDo: thread lock on stats recording maybe (mutex?)    
    this->stats->wait += truck_load_capacity * this->process_time;
    this->stats->served++;
    this->in_use = false;   
}

Client::Client(int id_client, int products_count) 
{  
    this->id_client = id_client;
    this->truck_load_capacity = products_count;
}
