#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <stdio.h>
#include <unistd.h>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;

typedef allocator<int, managed_shared_memory::segment_manager> ShmemAllocator;
typedef vector<int, ShmemAllocator> MyVector;

class AcUnit
{
public:
    int desiredTemperature[5];
    float roomTemperature[5] = {20, 20, 20, 20, 20};
    int roomIndex;
    int ind = 1;

    int j = 1;
    int exteriorTemperature;

    int copie;

    bool switchState = true;

    void switchIndexDelta()
    {

        ind++;

        if (ind == roomIndex + 1)
        {
            ind = 1;
        }

        tempDelta(roomTemperature[ind], desiredTemperature[ind]);
    }

    void switchOffState()
    {

        std::cout << "\n"
                  << "Termostat oprit "<<ind
                  << " \n";

        if (exteriorTemperature > roomTemperature[ind])
        {

            roomTemperature[ind] = roomTemperature[ind] + 0.1;

            sleep(1);

            std::cout << "Camera " << ind << " se incalzeste, afara e mai cald: " << roomTemperature[ind] << "\n";

            switchIndexDelta();
        }
        else if (exteriorTemperature < roomTemperature[ind])
        {

            roomTemperature[ind] = roomTemperature[ind] - 0.1;

            sleep(1);

            std::cout << "Camera " << ind << " se raceste, afara e mai frig: " << roomTemperature[ind] << "\n";

            switchIndexDelta();
        }
    }

    void tempDelta(double roomTemp, int targetTemp)
    {
        if ((targetTemp - roomTemp) >= 1)
        {
            Heating();
        }
        else if ((targetTemp - roomTemp) <= -1)
        {
            Cooling();
        }
        else
        {
            switchOffState();
        }
    }

    void Heating()
    {

        while (roomTemperature[ind] < desiredTemperature[ind])
        {
            roomTemperature[ind] = roomTemperature[ind] + 0.5;
            whatIsDoing(2);
            printLog();

            if ((roomTemperature[ind] + 0.6) >= desiredTemperature[ind])
            {
                roomTemperature[ind] = roomTemperature[ind] + 0.5;

                switchOffState();
            }

            checkInput();
            switchIndexDelta();
        }
    }

    void
    Cooling()
    {

        while (roomTemperature[ind] >= desiredTemperature[ind])
        {

            roomTemperature[ind] = roomTemperature[ind] - 0.5;
            whatIsDoing(1);
            printLog();
            if ((roomTemperature[ind]-0.6) <= desiredTemperature[ind])
            {
                roomTemperature[ind] = roomTemperature[ind] - 0.5;
                switchOffState();
            }
            checkInput();
            switchIndexDelta();
        }
    }

    void whatIsDoing(int n)
    {
        std::cout << "\n";
        switch (n)
        {
        case (1):
            std::cout
                << "Cooling"
                << "\n";
            break;
        case (2):
            std::cout
                << "Heating"
                << "\n";
            break;

        }
    }

    void checkInput()
    {

        managed_shared_memory segment(open_only, "MySharedMemoryNew");

        MyVector *myvector = segment.find<MyVector>("MySharedVector").first;

        roomIndex = (*myvector)[0];

        exteriorTemperature = (*myvector)[1];

        desiredTemperature[roomIndex] = (*myvector)[roomIndex + j];
    }

    void printLog()
    {

        for (int i = 1; i <= roomIndex; i++)
        {

            std::cout << "**********"
                      << "\n";
            std::cout << "Log camera: " << i;
            std::cout << "\n"
                      << "Temperatura curenta, camera: " << i << " este " << roomTemperature[i];
            std::cout << "\n";
            std::cout << "Temperatura dorita, camera: " << i << " este " << desiredTemperature[i] << "\n";
            std::cout << "Exterior temperature: " << exteriorTemperature << "\n";
            std::cout << "**********"
                      << "\n";
        }

        sleep(1);
    }

} unit;

int main()
{

    shared_memory_object::remove("MySharedMemoryNew");

    managed_shared_memory segment(open_or_create, "MySharedMemoryNew", 65536);

    const ShmemAllocator alloc_inst(segment.get_segment_manager());
    MyVector *myvector = segment.construct<MyVector>("MySharedVector")(alloc_inst);
    sleep(10);

    while (1)
    {

        unit.checkInput();

        unit.tempDelta(unit.roomTemperature[unit.ind], unit.desiredTemperature[unit.ind]);
    }

    return 0;
}