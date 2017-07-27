require 'socket'

sock = TCPSocket.open("localhost", ARGV.first || 12345)

while msg = STDIN.gets
	sock.write(msg)
	print(sock.gets)
end

sock.close