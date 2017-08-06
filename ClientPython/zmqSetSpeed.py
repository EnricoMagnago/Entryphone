#!/usr/bin/python

import zmq
import sys
import time

address = "127.0.0.1"
port = "6670"

#def sendRequest(type, dest, speed):
def sendRequest(request):
	context = zmq.Context()
  #print("Connecting to server...")
	socket = context.socket(zmq.REQ)
	socket.connect ("tcp://%s:%s" % (address,port))
	socket.RCVTIMEO = 30000	
	print("Sending request ", request)
	socket.send_string (request)
  #  Get the reply.
	message = socket.recv()
	print("Received reply: [", message, "]")	

if __name__=='__main__':
	#request = '{"cmd":"set_qr_pos","value":[{"id":1,"x":2.4,"y":0.8},{"id":2,"x":5.2,"y":1.6},{"id":3,"x":5.2,"y":3.6},{"id":4,"x":5.2,"y":5.6},{"id":5,"x":6.8,"y":5.6},{"id":6,"x":8.4,"y":5.6}]}'
	request = '{"cmd":"apri_porta"}'

	sendRequest(request)
	#time.sleep(1)
	#request = '{"cmd":"get_qr_pos"}'
	#sendRequest(request)

