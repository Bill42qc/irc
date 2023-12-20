#!/bin/sh
weechat -r "/server add irc host.docker.internal/6667 -notls" \
    weechat -r "/set irc.server.irc.password PASSWORD123" \
    weechat -r "/connect irc"