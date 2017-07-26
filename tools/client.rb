require 'socket'

sock = TCPSocket.open("localhost", ARGV.first || 12345)

print("prompt> ")
while msg = STDIN.gets
	sock.write(msg.strip)
	print(sock.gets)
  print("prompt> ")
end

sock.close