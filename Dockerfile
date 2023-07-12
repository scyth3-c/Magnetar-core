FROM debian:bullseye

RUN apt-get update
RUN apt-get install -y libstdc++-10-dev  g++-10 g++-10-multilib make 

RUN mkdir neody
WORKDIR /neody
COPY . ./

RUN ln -sf /usr/bin/g++-10 /usr/bin/g++
RUN make
RUN chmod 777 binary

EXPOSE 3000
# RUN adduser neody
# RUN chown neody ./binary 
# USER neody

CMD ./binary