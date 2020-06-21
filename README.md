<h1>ChatServer</h1>
Last Updated date [22/06/2020]
Contact [mudaliar.aditya@gmail.com](mailto:mudaliar.aditya@gmail.com)

A Basic Chat Application made with CUI client and server,the network is my major concern and priority,only one to one chat is supported, as udp is being used there is a chance for losses but still each packet which is not yet recieved a response is sent again
there is no handling for out of order packets.

I have developed/tested on debian,trying to keep it working across all linux distros


Features To be Added

 1. Resending from server{implemented}
 2. Shared memory Latency Tool{Dont think this will ever be implemented}
 3. removing logs{Always in Progress}
 4. OOPS{not necessary}
 5. Adding event to functions
 6. variable data length{Done}
 7. GUI to client{Pending}
 8. Cross platform client and server{Dont think server will be made cross platform,client code has to be merged}
 9. Encryption of messages(Encryption should be supplied through an shared object and configuration)(Implemented)
