# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    loop.py                                            :+:    :+:             #
#                                                      +:+                     #
#    By: quentinbeukelman <quentinbeukelman@stud      +#+                      #
#                                                    +#+                       #
#    Created: 2025/10/06 15:51:50 by quentinbeuk   #+#    #+#                  #
#    Updated: 2025/10/07 15:51:04 by quentinbeuk   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

import sys
import time
import signal

# Silance output
signal.signal(signal.SIGPIPE, signal.SIG_DFL)

print("Status: 200 OK")
print("Content-Type: text/plain")
print()
sys.stdout.flush()

i = 0
while True:
    print(f"Still running... {i}")
    sys.stdout.flush()
    i += 1
    time.sleep(1)
