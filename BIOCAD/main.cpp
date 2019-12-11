#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include "sources.h"

#define C 1389.38757

#define GROUP_SIZE 16

void performCalculation(const cl::Device &device,
                        const std::vector <cl_float4> &atomPositions,
                        const std::vector <cl_float> &atomCharges,
                        const std::vector <cl_int> &atomDegree,
                        const std::vector <cl_int> &atomEdges) {

    std::cout << std::endl
              << "-------------------------------------------------" << std::endl;
    std::cout << "Device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl
              << std::endl;

    const size_t atomCount = atomPositions.size();
    Timer timer;

    cl::Context context(device);
    cl::Program program(context, loadProgram("kernel.cl"));
    cl::CommandQueue queue(context, device);
    program.build({device}, "-cl-std=CL2.0");

    timer.start();

    // Prepare buffers
    cl::Buffer inputEdges =
            cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                       atomEdges.size() * sizeof(cl_int),
                       const_cast<cl_int *>(atomEdges.data()));

    cl::Buffer inputEdgesCount =
            cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                       atomDegree.size() * sizeof(cl_int),
                       const_cast<cl_int *>(atomDegree.data()));

    cl::Buffer inputQueueBuffer = cl::Buffer(
            context, CL_MEM_READ_WRITE, atomCount * atomCount * sizeof(cl_int));

    cl::Buffer inputPositions =
            cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                       atomPositions.size() * sizeof(cl_float4),
                       const_cast<cl_float4 *>(atomPositions.data()));

    cl::Buffer inputCharges =
            cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                       atomCharges.size() * sizeof(cl_float),
                       const_cast<cl_float *>(atomCharges.data()));

    std::vector <cl_float> outputPathScaleInit;
    outputPathScaleInit.assign(atomCount * atomCount, 1.0);
    cl::Buffer outputPathScale =
            cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                       outputPathScaleInit.size() * sizeof(cl_float),
                       outputPathScaleInit.data());

    int workGroupsCount = lround(atomCount / GROUP_SIZE + 0.5);
    std::vector <cl_float> outputEnergyHost;
    outputEnergyHost.resize(workGroupsCount * workGroupsCount);
    cl::Buffer outputEnergy =
            cl::Buffer(context, CL_MEM_READ_WRITE,
                       workGroupsCount * workGroupsCount * sizeof(cl_float));

    timer.stop("Time for preparing buffers");

    // Set kernel args
    cl::Kernel kernelBFS(program, "bfs");
    {
        int iArg = 0;
        kernelBFS.setArg(iArg++, inputEdges);
        kernelBFS.setArg(iArg++, inputEdgesCount);
        kernelBFS.setArg(iArg++, inputQueueBuffer);
        kernelBFS.setArg(iArg, outputPathScale);
    }

    cl::Kernel kernelCoulomb(program, "solve");
    {
        int iArg = 0;
        kernelCoulomb.setArg(iArg++, inputCharges);
        kernelCoulomb.setArg(iArg++, inputPositions);
        kernelCoulomb.setArg(iArg++, outputPathScale);
        kernelCoulomb.setArg(iArg, outputEnergy);
    }

    // Run kernel
    timer.start();
    queue.enqueueNDRangeKernel(kernelBFS, cl::NullRange, cl::NDRange(atomCount));
    queue.enqueueNDRangeKernel(kernelCoulomb, cl::NullRange,
                               cl::NDRange(atomCount, atomCount),
                               cl::NDRange(GROUP_SIZE, GROUP_SIZE));
    queue.enqueueReadBuffer(outputEnergy, CL_TRUE, 0,
                            workGroupsCount * workGroupsCount * sizeof(cl_float),
                            outputEnergyHost.data());
    queue.finish();

    // Sum energy
    float sumEnergy = 0;
    for (size_t i = 0; i < workGroupsCount * workGroupsCount; i++)
        sumEnergy += outputEnergyHost[i];
    timer.stop("Time for executing");

    std::cout << "Energy: " << sumEnergy << std::endl;
}


int main() {
    try {
        std::vector <cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if (platforms.empty()) {
            std::cout << "OpenCL platform not found\n";
            return -1;
        }

        std::vector <cl::Device> devices;

        for (const auto &platform : platforms) {
            std::vector <cl::Device> devs;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &devs);
            devices.insert(devices.begin(), devs.begin(), devs.end());
        }

        auto atomPositions = loadAtoms("data/atoms.txt");
        auto atomCharges = loadCharges("data/charges.txt");
        auto[atomEdges, atomDegree] =
        convertMatrix(loadBounds("data/bonds.txt", atomPositions.size()));

        for (const auto &device : devices) {
            performCalculation(device, atomPositions, atomCharges, atomDegree,
                               atomEdges);
        }

    } catch (const cl::Error &err) {
        std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")"
                  << std::endl;
    }

    return 0;
}