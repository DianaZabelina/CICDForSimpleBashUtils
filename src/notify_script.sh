#!/bin/bash

source .env

api_url="https://api.telegram.org/bot${bot_token}/sendmessage"

if [ "$CI_JOB_STATUS" == "success" ]; then
    message="Этап $CI_JOB_STAGE успешно завершен"
else
    message="Этап $CI_JOB_STAGE завершен с ошибкой"
fi

curl -s -X POST "$api_url" -d chat_id="$chat_id" -d text="$message"