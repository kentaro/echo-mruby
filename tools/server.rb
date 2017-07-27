require 'socket'

server = TCPServer.new(ARGV[1] || 12345)
client = server.accept

while msg = client.gets
  client.puts(msg)
end

client.close
server.close