# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: qbeukelm <qbeukelm@student.42.fr>            +#+                      #
#                                                    +#+                       #
#    Created: 2025/08/11 09:30:12 by qbeukelm      #+#    #+#                  #
#    Updated: 2025/10/14 01:35:47 by quentinbeuk   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

BOLD				:= \033[1m
RED					:= \033[31;1m
GREEN				:= \033[32;1m
YELLOW				:= \033[33;1m
BLUE				:= \033[34;1m
RESET				:= \033[0m

# ---------------- Compiler ----------------
CXX       := c++
CXXFLAGS  := -std=c++17 -MMD -MP -Iinc -Itests -Werror -Wextra -Wall

# ---------------- Targets -----------------
NAME				:= webserv
TEST_BIN  			:= unit_tests
UPLOADS_DIRECTORY	:= var/www/uploads

# ---------------- Dirs --------------------
DIR_OBJ   := obj

# ---------------- Sources -----------------
LIB_SRCS := \
src/config/config_parser/ConfigParser.cpp \
src/config/config_parser/ConfigParserUtils.cpp \
src/config/config_parser/parseAllowMethods.cpp \
src/config/config_parser/parseAutoIndex.cpp \
src/config/config_parser/parseCgi.cpp \
src/config/config_parser/parseErrorPage.cpp \
src/config/config_parser/parseIndex.cpp \
src/config/config_parser/parseListen.cpp \
src/config/config_parser/parseMaxBody.cpp \
src/config/config_parser/parseName.cpp \
src/config/config_parser/parseRedirect.cpp \
src/config/config_parser/parseRoot.cpp \
src/config/config_parser/parseUpload.cpp \
src/config/config_parser/TokenStream.cpp \
\
src/config/location_helpers/findLocation.cpp \
src/config/location_helpers/getCgiByExtension.cpp \
\
src/config/models/CGI.cpp \
\
src/config/Location.cpp \
src/config/Server.cpp \
src/config/ServerConfig.cpp \
\
src/http/models/ContentType.cpp \
src/http/models/File.cpp \
src/http/models/HttpMethod.cpp \
src/http/models/HttpRequest.cpp \
src/http/models/HttpResponse.cpp \
src/http/models/HttpStatus.cpp \
src/http/models/Mime.cpp \
src/http/models/MultipartFile.cpp \
src/http/models/ParseContext.cpp \
src/http/models/RequestParseStatus.cpp \
\
src/http/request_parser/handleBodyContentLength.cpp \
src/http/request_parser/handleChunkData.cpp \
src/http/request_parser/handleChunkSize.cpp \
src/http/request_parser/handleComplete.cpp \
src/http/request_parser/handleStartLineAndHeaders.cpp \
src/http/request_parser/parserHeaders.cpp \
src/http/request_parser/parserStartLine.cpp \
src/http/request_parser/parserUtils.cpp \
\
src/http/requestHandler/handler_utils/composeMultiPartData.cpp \
src/http/requestHandler/handler_utils/generateAutoIndexResponse.cpp \
src/http/requestHandler/handler_utils/generateRedirectResponse.cpp \
src/http/requestHandler/handler_utils/generateUploadFile.cpp \
src/http/requestHandler/handler_utils/makeHtmlPageHeader.cpp \
\
src/http/requestHandler/handleDelete.cpp \
src/http/requestHandler/handleGet.cpp \
src/http/requestHandler/handlePost.cpp \
src/http/requestHandler/makeError.cpp \
\
src/http/RequestHandler.cpp \
src/http/RequestParser.cpp \
\
src/log/Logger.cpp \
src/log/printUtils.cpp \
\
src/serve/cgi/cgiHelpers.cpp \
src/serve/cgi/CgiStdinPollable.cpp \
src/serve/cgi/CgiStdoutPollable.cpp \
\
src/serve/CgiPollable.cpp \
src/serve/Connection.cpp \
src/serve/EventLoop.cpp \
src/serve/IOPollable.cpp \
src/serve/Listener.cpp \
src/serve/ShutdownPollable.cpp \
src/serve/WebServer.cpp \
\
src/utils/utils.cpp \

# ---------------- Main -----------------
APP_SRCS := $(LIB_SRCS) src/main.cpp

# ---------------- Tests -----------------
TEST_SRCS := \
tests/config/test_findLocation.cpp \
\
tests/http/request_handler/test_generateUploadFile.cpp \
\
tests/http/request_parser/test_handleBodyContentLength.cpp \
tests/http/request_parser/test_handleChunkedData.cpp \
tests/http/request_parser/test_handleChunkedSize.cpp \
tests/http/request_parser/test_handleStartLineAndHeaders.cpp \
tests/http/request_parser/test_httpMethod.cpp \
tests/http/request_parser/test_parserStartLine.cpp \
tests/http/request_parser/test_requestParserStep.cpp \
\
tests/integration/test_serverIntegrationTest.cpp \
\
tests/serve/test_listener.cpp \
tests/serve/test_location.cpp \

# ---------------- Derived -----------------
APP_OBJS  := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(APP_SRCS))
LIB_OBJS  := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(LIB_SRCS))
TEST_OBJS := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(TEST_SRCS))

DEPENDS   := $(APP_OBJS:.o=.d) $(TEST_OBJS:.o=.d)

# ---------------- Rules -------------------
all: $(NAME)

$(NAME): $(APP_OBJS)
	@echo "$(BLUE)\nLinking $(NAME)...$(RESET)"
	@$(CXX) $(APP_OBJS) $(LDFLAGS) -o $@
	@mkdir -p $(UPLOADS_DIRECTORY)
	@echo "$(GREEN)\nDone\n$(RESET)"

# Build + run unit tests
test: $(TEST_BIN)
	@./$(TEST_BIN)

$(TEST_BIN): $(LIB_OBJS) $(TEST_OBJS)
	@echo "$(BLUE)\nLinking $(TEST_BIN)...$(RESET)"
	@$(CXX) $(LIB_OBJS) $(TEST_OBJS) $(LDFLAGS) -o $@
	@echo "$(GREEN)\nUnit tests ready\n$(RESET)"

# Generic compile rule (app + tests)
$(DIR_OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(BLUE)\nCleaning ...$(RESET)"
	@rm -rf $(DIR_OBJ) $(TEST_BIN)
	@echo "$(GREEN)$(BOLD)\nAll clean!\n$(RESET)"

fclean: clean
	@rm -rf $(UPLOADS_DIRECTORY)
	@rm -f $(NAME)

re: fclean all

-include $(DEPENDS)

.PHONY: all clean fclean re test
