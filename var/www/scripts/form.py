# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    form.py                                            :+:    :+:             #
#                                                      +:+                     #
#    By: quentinbeukelman <quentinbeukelman@stud      +#+                      #
#                                                    +#+                       #
#    Created: 2025/09/27 13:55:40 by quentinbeuk   #+#    #+#                  #
#    Updated: 2025/10/07 15:18:28 by quentinbeuk   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

import os
import sys
import json
import urllib.parse
import datetime
import cgi

# CONFIG
# ____________________________________________________________________________
DATA_DIRECTORY  = "var/www/data"
DATA_FILE = os.path.join(DATA_DIRECTORY, "records.jsonl")
MAX_POST_SIZE = 1_000_000  # 1MB

# HELPERS
# ____________________________________________________________________________
def respond(status="200 OK", ctype="text/html"):
    print(f"Status: {status}")
    print(f"Content-Type: {ctype}")
    print()

def html_replace_special_chars(s: str) -> str:
    return (s.replace("&","&amp;").replace("<","&lt;")
             .replace(">","&gt;").replace('"',"&quot;").replace("'","&#39;"))

def check_data_directory():
    try:
        os.makedirs(DATA_DIRECTORY, exist_ok=True)
    except Exception:
        pass

def parse_form_query_string(query_string: str) -> dict:
    return urllib.parse.parse_qs(query_string, keep_blank_values=True, strict_parsing=False)

def does_record_match(record: dict, filters: dict) -> bool:
    for field, want_values in filters.items():
        has_record = record.get(field)
        if has_record is None:
            return False
        if isinstance(has_record, list):
            if not any(value in has_record for value in want_values):
                return False
        else:
            if has_record not in want_values:
                return False
    return True

def load_records():
    if not os.path.exists(DATA_FILE):
        return []
    records = []
    with open(DATA_FILE, "r", encoding="utf-8") as file:
        for line in file:
            line = line.strip()
            if not line:
                continue
            try:
                records.append(json.loads(line))
            except Exception:
                continue
    return records

def save_record(data: dict):
    check_data_directory()
    record = {
        "ts": datetime.datetime.utcnow().isoformat(timespec="seconds") + "Z",
        **data
    }
    with open(DATA_FILE, "a", encoding="utf-8") as file:
        file.write(json.dumps(record, ensure_ascii=False) + "\n")
    return record

def render_records(title: str, records: list):
    print(f"<h2>{html_replace_special_chars(title)}</h2>")
    if not records:
        print("<p><em>No records.</em></p>")
        return
    print("<ol>")
    for record in records:
        timestamp = html_replace_special_chars(record.get("ts",""))
        print(f"<li><strong>{timestamp}</strong><ul>")
        for field_name, field_value in record.items():
            if field_name == "ts":
                continue
            values = field_value if isinstance(field_value, list) else [field_value]
            for val in values:
                print(f"<li>{html_replace_special_chars(field_name)} = {html_replace_special_chars(str(val))}</li>")
        print("</ul></li>")
    print("</ol>")

def parse_post_name_age():
    form = cgi.FieldStorage(fp=sys.stdin, environ=os.environ, keep_blank_values=True)

    fields = {}
    name_values = []
    age_values  = []

    try:
        name_values = form.getlist("name")
    except Exception:
        name_values = []
    try:
        age_values = form.getlist("age")
    except Exception:
        age_values = []

    if name_values:
        fields["name"] = name_values
    if age_values:
        fields["age"]  = age_values

    return fields

# MAIN
# ____________________________________________________________________________
def main():
    method = os.environ.get("REQUEST_METHOD", "GET").upper()

    if method == "POST":
        try:
            content_length = int(os.environ.get("CONTENT_LENGTH", "0") or 0)
        except ValueError:
            content_length = 0
        if content_length > MAX_POST_SIZE:
            respond("413 Payload Too Large")
            print("<h2>Payload too large</h2>")
            print('<p><a href="/scripts/form.py">Back</a></p>')
            return

        fields = parse_post_name_age()
        saved = save_record(fields)

        respond("200 OK", "text/html")
        print("<h2>POST saved</h2>")
        if fields:
            print("<p>Stored the following fields:</p><ul>")
            for field_name, field_values in fields.items():
                for val in field_values:
                    print(f"<li>{html_replace_special_chars(field_name)} = {html_replace_special_chars(val)}</li>")
            print("</ul>")
        else:
            print("<p><em>No name/age fields were provided.</em></p>")

        print('<p><a href="/scripts/form.py">View recent records</a></p>')

    elif method == "GET":
        respond("200 OK", "text/html")
        filters = parse_form_query_string(os.environ.get("QUERY_STRING", ""))
        records = load_records()
        if filters:
            filtered = [r for r in records if does_record_match(r, filters)]
            render_records("GET records (filtered)", filtered[-20:])
        else:
            render_records("GET recent records", records[-20:])

    else:
        respond("405 Method Not Allowed", "text/html")
        print(f"<h2>Unsupported method: {html_replace_special_chars(method)}</h2>")

if __name__ == "__main__":
    main()
