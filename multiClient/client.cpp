#include <cstdlib>
#include <string>
#include <iostream>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;

typedef allocator<int, managed_shared_memory::segment_manager> ShmemAllocator;
typedef vector<int, ShmemAllocator> MyVector;

int main()
{
    int index = 2;
    int desiredTemperature;
    int exteriorTemperature;
    int client;
    int contor = 1;
    int copie;
    bool visited = 0;
    int i = 2;
    int Pstart = 0;
    int Pend = 2;

    int ctr;

    int users;

    managed_shared_memory segment(open_only, "MySharedMemoryNew");

    MyVector *myvector = segment.find<MyVector>("MySharedVector").first;

    if (myvector->empty() == false)
    {
        ctr = (*myvector)[0];
        contor = contor + ctr;
    }

    while (1)
    {
        std::cout << "\nInsert desired temperature in interval [16 , 34]:  ";
        std::cin >> desiredTemperature;
        std::cout << "Insert exterior temperature in interval [-10 , 40]: ";
        std::cin >> exteriorTemperature;

        if (desiredTemperature >= 16 && desiredTemperature <= 34 && exteriorTemperature >= (-10) && exteriorTemperature <= 40)
        {

            if (visited == 1)
            {

                myvector->erase(std::next(myvector->begin(), Pstart),
                                std::next(myvector->begin(), Pend));

                myvector->erase(std::next(myvector->begin(), i - 2));
            }

            // index

            if (myvector->empty())
            {
                myvector->insert(myvector->begin() + 0, contor);
                myvector->insert(myvector->begin() + 1, exteriorTemperature);
            }

            else
            {
                myvector->erase(std::next(myvector->begin(), Pstart),
                                std::next(myvector->begin(), Pend));

                myvector->insert(myvector->begin() + 0, contor);
                myvector->insert(myvector->begin() + 1, exteriorTemperature);
            }

            if (visited == 0)
            {
                while (1)
                {
                    if (i == (*myvector).size())
                    {

                        myvector->insert(myvector->begin() + i, desiredTemperature);
                        visited = 1;

                        break;
                    }
                    i++;
                }
            }
            else
            {

                myvector->insert(myvector->begin() + i, desiredTemperature);
            }

            for (int i = 0; i < (*myvector).size(); i++)
            {

                std::cout << (*myvector)[i] << " ";
            }
        }
    }
    return 0;
}
