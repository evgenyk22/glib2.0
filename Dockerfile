FROM gcc:4.9
COPY . /usr/src/proj
WORKDIR /usr/src/proj
#RUN apt-get install cmake 
CMD ["make"]
