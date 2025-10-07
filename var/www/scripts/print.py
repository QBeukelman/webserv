# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    print.py                                           :+:    :+:             #
#                                                      +:+                     #
#    By: quentinbeukelman <quentinbeukelman@stud      +#+                      #
#                                                    +#+                       #
#    Created: 2025/10/07 11:06:59 by quentinbeuk   #+#    #+#                  #
#    Updated: 2025/10/07 15:38:15 by quentinbeuk   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

import os
import sys

print("Status: 200 OK")
print("Content-Type: text/plain; charset=utf-8")
print()
print("Hello from CGI!")
print("Method:", os.environ.get("REQUEST_METHOD"))
print("Content-length:", os.environ.get("CONTENT_LENGTH"))
print("Body:", sys.stdin.read())
