#include <vld.h>
#include <liblas/cstdint.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/detail/timer.hpp>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <utility>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <cassert>
using namespace std;


template <typename T>
inline std::ostream& operator<<(std::ostream& os, typename std::pair<T, T> const& p)
{
    os << p.first << "   " << p.second;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, liblas::LASPoint const& p)
{
    os << std::fixed << std::setprecision(6)
       << "\nx: " << p.GetX()
       << "\ny: " << p.GetY()
       << "\nz: " << p.GetZ()
       << "\nint: " << p.GetIntensity()
       << "\nrn: " << p.GetReturnNumber()
       << "\nnor: " << p.GetNumberOfReturns()
       << "\nsd: " << p.GetScanDirection()
       << "\neofl: " << p.GetFlightLineEdge()
       << "\ncls: " << p.GetClassification()
       << std::endl;
    return os;
}


int main()
{
    try
    {
        char const* name = "d:\\data\\lidar\\LDR030828_213450_0.LAS"; 
        //char const* name = "d:\\data\\lidar\\Sample_LiDAR_LAS_File.las";
        //char const* name = "d:\\data\\lidar\\iowa\\04164492.las";
        //char const* name = "d:\\dev\\liblas\\_svn\\trunk\\test\\data\\TO_core_last_clip.las";
        //char const* name = "d:\\data\\lidar\\gilmer\\000001.las";
        
        std::ifstream ifs(name, ios::in | ios::binary);
        liblas::LASReader reader(ifs);
        
        cout << "File: " << name << '\n';
        cout << "Version: " << reader.GetVersion() << '\n';

        cout << "Signature: " << reader.GetHeader().GetFileSignature() << '\n';
        cout << "Points count: " << reader.GetHeader().GetPointRecordsCount() << '\n';

        liblas::detail::Timer t;
        t.start();
        
        typedef std::pair<double, double> minmax_t;
        minmax_t mx;
        minmax_t my;
        minmax_t mz;
        liblas::uint32_t i = 0;
        while (reader.ReadPoint())
        {
            liblas::LASPoint const& p = reader.GetPoint();

            mx.first = std::min<double>(mx.first, p.GetX());
            mx.second = std::max<double>(mx.second, p.GetX());
            my.first = std::min<double>(my.first, p.GetY());
            my.second = std::max<double>(my.second, p.GetY());
            mz.first = std::min<double>(mz.first, p.GetZ());
            mz.second = std::max<double>(mz.second, p.GetZ());
            
            // Warning: Printing zillion of points may take looong time
            //cout << i << ". " << p << '\n';
            i++;
        }
        double const d = t.stop();

        if (reader.GetHeader().GetPointRecordsCount() != i)
            throw std::runtime_error("read incorrect number of point records");

        cout << "Read points: " << i << " (" << d << ")\n"
            << std::fixed << std::setprecision(6)
            << "\nX: " << mx
            << "\nY: " << my
            << "\nZ: " << mz    
            << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}