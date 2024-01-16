import telnetlib
import threading
import time
from multiprocessing import Value

HOST = "127.0.0.1"
PORT = 6667
PASSWORD = "PASSWORD123"
DELAY_BETWEEN_CLIENTS = 0.15  # Adjust the delay as needed

def check_registration_confirmation(response):
    return "001" in response

def send_command(tn, command):
    tn.write(command.encode('utf-8'))
    response = tn.read_until(b"\r\n", timeout=1).decode('utf-8')
    return response

def connect_and_register(client_id, counter, total_clients, connections):
    tn = telnetlib.Telnet(HOST, PORT)

    pass_command = "PASS {}\r\n".format(PASSWORD)
    nick_command = "NICK test{}\r\n".format(client_id)
    user_command = "USER test{} 0 * :test{}\r\n".format(client_id, client_id)

    commands = [pass_command, nick_command, user_command]

    for command in commands:
        response = send_command(tn, command)

    if check_registration_confirmation(response):
        print("Client {}: \033[92mS\033[0m Registration successful".format(client_id))
    else:
        print("Client {}: \033[91mX\033[0m Registration failed".format(client_id))
        print("Output message: {}".format(response))

    with counter.get_lock():
        counter.value += 1
        connections.append(tn)

def start_telnet_clients(num_clients):
    threads = []
    counter = Value('i', 0)
    connections = []

    for i in range(1, num_clients + 1):
        thread = threading.Thread(target=connect_and_register, args=(i, counter, num_clients, connections))
        threads.append(thread)
        thread.start()

        time.sleep(DELAY_BETWEEN_CLIENTS)

    for thread in threads:
        thread.join()

    # Close all connections after all clients have connected
    for connection in connections:
        connection.close()

def connection_irc(client_id, counter, total_clients, connections):
    tn = telnetlib.Telnet(HOST, PORT)

    pass_command = "PASS {}\r\n".format(PASSWORD)
    nick_command = "NICK test{}\r\n".format(client_id)
    user_command = "USER test{} 0 * :test{}\r\n".format(client_id, client_id)

    commands = [pass_command, nick_command, user_command]
    for command in commands:
        response = send_command(tn, command)

    with counter.get_lock():
        counter.value += 1
        connections.append(tn)
    
    return tn

def check_join_response(response):
    return "JOIN" in response

def check_priv_response(response):
    return "PRIVMSG" in response

def join_valid_channel(tn, client_id):
    join_command = "JOIN #testChannel\r\n"

    response = send_command(tn, join_command)
    if check_join_response(response):
        print("Client {}: \033[92mS\033[0m Join Successfull".format(client_id))
    else:
        print("Client {}: \033[91mX\033[0m Join Fail".format(client_id))
    time.sleep(1)
    if client_id == 1:
        privCommand = "PRIVMSG #testChannel :TESTING_THIS_CHANNEL\r\n"
        send_command(tn, privCommand)
    else:
        priv_response = tn.read_until(b"\r\n", timeout=1).decode('utf-8')
        if check_priv_response(priv_response):
            print("Client {}: \033[92mS\033[0m received message Successful".format(client_id))
        else:
            print("Client {}: \033[91mX\033[0m received message Fail {}".format(client_id))
        

def join_bad_channel(tn, client_id):
    join_command2 = "JOIN test\r\n"
    join_command4 = "JOIN\r\n"

    response = send_command(tn, join_command2)
    if check_join_response(response):
        print("Client {}: \033[91mX\033[0m Join Fail".format(client_id))
    else:
        print("Client {}: \033[92mS\033[0m Join Successfull".format(client_id))
    response = send_command(tn, join_command4)
    if check_join_response(response):
        print("Client {}: \033[91mX\033[0m Join Fail".format(client_id))
    else:
        print("Client {}: \033[92mS\033[0m Join Successfull".format(client_id))

def connect_and_register_join(client_id, counter, total_clients, connections):
    tn = connection_irc(client_id, counter, total_clients, connections)
    join_bad_channel(tn, client_id)
    join_valid_channel(tn, client_id)

    with counter.get_lock():
        counter.value += 1
        connections.append(tn)

def join_test(num_clients):
    threads = []
    counter = Value('i', 0)
    connections = []

    for i in range(1, num_clients + 1):
        thread = threading.Thread(target=connect_and_register_join, args=(i, counter, num_clients, connections))
        threads.append(thread)
        thread.start()

        time.sleep(DELAY_BETWEEN_CLIENTS)

    for thread in threads:
        thread.join()

    # Close all connections after all clients have connected
    for connection in connections:
        connection.close()

if __name__ == "__main__":
    start_telnet_clients(75)  # Connect 50 clients with a shorter delay
    #print("TESTING JOIN\n")
    #time.sleep(0.5)
    #join_test(5)
