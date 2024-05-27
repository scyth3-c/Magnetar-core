FROM debian:bookworm

RUN apt-get update
RUN apt-get install -y g++ make

RUN mkdir neody
WORKDIR /neody
COPY . .

RUN make
RUN chmod 777 binary 

EXPOSE 8080

CMD ./binary