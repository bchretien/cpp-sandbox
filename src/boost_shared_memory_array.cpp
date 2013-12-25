// Based on:
// http://www.boost.org/doc/libs/1_55_0/doc/html/interprocess/sharedmemorybetweenprocesses.html#interprocess.sharedmemorybetweenprocesses.sharedmemory.shared_memory_a_simple_example

#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <cstring>

int main (int argc, char *argv[])
{
    const size_t ARRAY_SIZE = 4;

    using namespace boost::interprocess;
    try
    {
        // Parent process
        if (argc == 1)
        {
            // Remove shared memory on construction and destruction
            struct shm_remove
            {
                 shm_remove () { shared_memory_object::remove ("MySharedMemory"); }
                ~shm_remove () { shared_memory_object::remove ("MySharedMemory"); }
            } remover;

            // Create a new segment with 4 integers
            shared_memory_object shm (create_only, "MySharedMemory", read_write);
            shm.truncate (ARRAY_SIZE * sizeof (int));

            // Map the whole shared memory in this process
            mapped_region region(shm, read_write);

            // Write all the memory to 0
            std::memset (region.get_address (), 0, region.get_size ());

            int* ar = static_cast<int*> (region.get_address());

            for (size_t i = 0; i < ARRAY_SIZE; ++i)
            {
                ar[i] = static_cast<int>(i);
                std::cout << ar[i] << std::endl;
            }

            // Launch child process
            std::string s (argv[0]);
            s += " child ";
            if (0 != std::system (s.c_str ()))
                return 1;

        }
        else // Child process
        {
            // Open already created shared memory object.
            shared_memory_object shm (open_only, "MySharedMemory", read_only);

            // Map the whole shared memory in this process
            mapped_region region (shm, read_only);

            int* ar = static_cast<int*> (region.get_address());

            // Print the vector
            for (size_t i = 0; i < ARRAY_SIZE; ++i)
                std::cout << ar[i] << std::endl;
        }
    }
    catch (interprocess_exception &ex)
    {
        std::cout << ex.what () << std::endl;
        return 1;
    }
    return 0;
}
