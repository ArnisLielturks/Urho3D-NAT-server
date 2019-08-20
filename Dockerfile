FROM ubuntu:18.04
MAINTAINER Arnis Lielturks <arnis@example.com>

# Install all the dependencies
RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y libgl1-mesa-dev git cmake g++ libx11-dev \
    && apt-get purge --auto-remove -y && apt-get clean

# Clone the Urho repo and remove all the samples
RUN git clone https://github.com/urho3d/Urho3D.git /Urho3D && \
	cd /Urho3D/Source/Samples && \
	# Remove all samples
	ls -la | awk '{print $9}' | grep _ | xargs rm -r

# Copy NAT server files
COPY Source/Samples/NATServer /Urho3D/Source/Samples/NATServer
COPY Source/ThirdParty/SLikeNet/CMakeLists.txt /Urho3D/Source/ThirdParty/SLikeNet/CMakeLists.txt

# Compile the engine with NAT server sample
RUN cd /Urho3D && \
	g++ --version && \
    bash script/cmake_generic.sh build -DURHO3D_SAMPLES=1 -DURHO3D_TOOLS=0 -DURHO3D_HASH_DEBUG=0 -DURHO3D_PROFILING=0 -DURHO3D_64BIT=1 -DURHO3D_TESTING=0 -DURHO3D_DEPLOYMENT_TARGET=generic && \
    cd build && make -j $(nproc) && \
    cd ..

WORKDIR /var/source

# Create the actual image
FROM ubuntu:18.04
MAINTAINER Arnis Lielturks <arnis.lielturks@gmail.com>

RUN apt-get update \
    && apt-get install -y supervisor libgl1-mesa-dev libc6 \
    && apt-get purge --auto-remove -y && apt-get clean

# Pack everything we need for the NATServer
COPY --from=0 /Urho3D/bin/CoreData /var/app/CoreData
COPY --from=0 /Urho3D/bin/Data /var/app/Data
COPY --from=0 /Urho3D/build/bin/NATServer /var/app/NATServer
COPY CommandLine.txt /var/app/Data/CommandLine.txt

WORKDIR /var/app

EXPOSE 61111/udp

CMD cd /var/app && ./NATServer
