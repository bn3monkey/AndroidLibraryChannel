# How to communicate libraries

# 1. Requirement

## 1.1 Role

1. Data Destination library requests for Data Source library to execute some function.
2. Data source library send data to Data destion library
3. Data source library and Data destination library are built indenpendently.

## 1.2 RPC

data destination library requests to run function in a data source library
the data source library processes request from the data destination library and send result to the destination library

## 1.3 Data Stream

data source library sends data to a data destination library intervally.
the data destination library receive data and process it 

# 2. Socket

## Socket Client
- blocking io
  - wait for server packet
- easy to implement

## Socket Server
- nonblocking-io
  - do not wait for client packet
  - only be procssed when client packet arrives
- execution routine
  - receive command -> excute -> send result
  - run repeatedly
- relatively difficult to implement than socket client

## Use-Case

### RPC

1. initialize server socket for data source library 
2. Run command execution routine for data source library
3. initialize client socket for data destination library
4. data destination library sends command through client socket
5. command execution routine receives command through server socket 
6. command execution routine processes function according to the command
7. command execution routine send result through server socket
8. data destination library receives result through client socket

### Data Stream

1. initialize server socket for data destination library
2. Run data process routine for data destination library
3. initialize client socket for data source library
4. data destination library sends data through client socket
5. data process routine receives data through server socket 
6. data process routine execute function according to the data


## Consideration

- packet size must be fixed.
  - tcp has no data boundary
    - multiple sent data can be received with just one recv call.
- How to implement RPC
  - Using exsiting RPC
    - e.g.) gRPC
    - increasing open source management cost
      - tracking version at every release
      - tracking known-bug at every release
    - learning curve
  - what language to be selected for execution routine
    - native
      - less overhead (+)
      - parsing packet is easy (+)
        - extracting function name
        - extracting parameters with type conversion
      - use native thread (-)
      - difficult to debug (-)
    - kotlin
      - easy to debug (+)
      - additional jni-call needed (-)
      - parsing packet is difficult (-)
 - Difference with native libary interface
   - how to deal with output parameters. it should be provided through another way
   - how to deal with unsigned type. kotlin does not fully support unsigned type.