#include "task.h"
#include <thread>
#include <cstdlib> 
#include <iostream> 

using namespace std;


int main(int argc, char* argv[]) 
{
    setlocale(LC_ALL, "Russian"); 


    int one_time_period = 10;
    int cur_time;
    int number_of_clients = 0;
    int deque_size = 10; //максимальная длина очереди
    int product_process_time = 10; //скорость обработки товара на кассе
    int mean_products = 10; //среднее колличество продуктов в тележке
    int threads_count = 3; //количество касс
    double client_intensivity = (double)1 / deque_size; //интенсивность потока покупателей
    int TOTAL = 5000;

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    Shop shop = Shop(threads_count, deque_size, product_process_time);
    shop.Start();
    int Per_time_unit;
    int product_count;

    while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - begin).count() < TOTAL) 
    {
        Per_time_unit = rand() % int(2 * one_time_period * client_intensivity);
        for (int i = 0; i < Per_time_unit; i++) 
        {
            product_count = rand() % (2 * mean_products);
            Client client = Client(number_of_clients, product_count);   
            shop.add_to_queue(client);
            number_of_clients++;
        }
        this_thread::sleep_for(chrono::milliseconds(one_time_period));
    }
    shop.Stop();

    int sr = shop.stats.queue / shop.stats.observations_count;
    int me = shop.stats.wait / shop.stats.served;
    double re = (double)shop.stats.rejected / (shop.stats.served + shop.stats.rejected);
    double rel = 1 - (double)shop.stats.rejected / (shop.stats.served + shop.stats.rejected);
    double cl = client_intensivity * (1 - (double)shop.stats.rejected / (shop.stats.served + shop.stats.rejected));  


    cout << "1) Колличество обслуженных клиентов: " << shop.stats.served << "\n";
    cout << "2) Колличество необслуженных клиентов: " << shop.stats.rejected << "\n";  
    cout << "3) Средняя длина очереди: " << sr << "\n";  
    cout << "4) Среднее ожидание клиента : " << me << "\n";
    cout << "--------------------------------------------------------------------" << "\n"; 
    cout << "5) Вероятность отказа: " << re << "\n";
    cout << "6) Относительную пропускная способность магазина: " << rel << "\n";
    cout << "7) Абсолютная пропускная способность: " << cl << endl;

    return 0;
  
}
