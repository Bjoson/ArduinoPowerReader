import socket

ARDUINO_IP = "192.168.0.24"  # The IP of your Arduino
ARDUINO_PORT = 6438

def send_command(command):
    """
    Connects to the Arduino, sends a command, reads one line of response, returns it.
    """
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ARDUINO_IP, ARDUINO_PORT))
        # Send the command + newline
        to_send = command + "\n"
        s.sendall(to_send.encode('utf-8'))

        # Read the response (up to 1 KB)
        data = s.recv(1024)
        return data.decode('utf-8').strip()

if __name__ == "__main__":
    # Example usage
    pulses_resp = send_command("P")
    print("Pulse count received:", pulses_resp)

    # door_resp = send_command("D")
    # print("Door toggle response:", door_resp)

    u_resp = send_command("Q")
    print("Unknown command response:", u_resp)
