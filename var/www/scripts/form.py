# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    form.py                                            :+:    :+:             #
#                                                      +:+                     #
#    By: quentinbeukelman <quentinbeukelman@stud      +#+                      #
#                                                    +#+                       #
#    Created: 2025/09/27 13:55:40 by quentinbeuk   #+#    #+#                  #
#    Updated: 2025/09/28 11:59:26 by quentinbeuk   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env python3
import os, sys, json, urllib.parse, datetime

# ==== Config ====
DATA_DIR  = "var/www/data"
DATA_FILE = os.path.join(DATA_DIR, "records.jsonl")  # one JSON object per line
MAX_POST_BYTES = 1_000_000

def print_headers():
    print("Content-Type: text/html\r\n\r\n", end="")

def html_escape(s: str) -> str:
    return (s.replace("&","&amp;").replace("<","&lt;")
             .replace(">","&gt;").replace('"',"&quot;").replace("'","&#39;"))

def ensure_data_dir():
    try:
        os.makedirs(DATA_DIR, exist_ok=True)
    except Exception:
        pass

def read_post_body():
    try:
        length = int(os.environ.get("CONTENT_LENGTH", 0))
    except ValueError:
        length = 0
    if length < 0: length = 0
    if length > MAX_POST_BYTES:
        length = MAX_POST_BYTES
    return sys.stdin.read(length)

def parse_form_qs(qs: str) -> dict:
    return urllib.parse.parse_qs(qs, keep_blank_values=True, strict_parsing=False)

def record_matches(rec: dict, filters: dict) -> bool:
    for k, want_vals in filters.items():
        have = rec.get(k)
        if have is None:
            return False
        if isinstance(have, list):
            if not any(v in have for v in want_vals):
                return False
        else:
            if have not in want_vals:
                return False
    return True

def load_records():
    if not os.path.exists(DATA_FILE):
        return []
    records = []
    with open(DATA_FILE, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                records.append(json.loads(line))
            except Exception:
                continue
    return records

def save_record(data: dict):
    ensure_data_dir()
    rec = {
        "ts": datetime.datetime.utcnow().isoformat(timespec="seconds") + "Z",
        **data
    }
    with open(DATA_FILE, "a", encoding="utf-8") as f:
        f.write(json.dumps(rec, ensure_ascii=False) + "\n")
    return rec

def render_records(title: str, records: list):
    print(f"<h2>{html_escape(title)}</h2>")
    if not records:
        print("<p><em>No records.</em></p>")
        return
    print("<ol>")
    for rec in records:
        ts = html_escape(rec.get("ts",""))
        print(f"<li><strong>{ts}</strong><ul>")
        for k, v in rec.items():
            if k == "ts": 
                continue
            vals = v if isinstance(v, list) else [v]
            for val in vals:
                print(f"<li>{html_escape(k)} = {html_escape(str(val))}</li>")
        print("</ul></li>")
    print("</ol>")

def main():
    print_headers()
    method = os.environ.get("REQUEST_METHOD", "GET").upper()

    if method == "POST":
        body = read_post_body()
        fields = parse_form_qs(body)
        saved = save_record(fields)
        print("<h2>POST saved</h2>")
        print("<p>Stored the following fields:</p><ul>")
        for k, vals in fields.items():
            for v in vals:
                print(f"<li>{html_escape(k)} = {html_escape(v)}</li>")
        print("</ul>")
        print("<p><a href=\"/scripts/form.py\">View recent records</a></p>")

    elif method == "GET":
        filters = parse_form_qs(os.environ.get("QUERY_STRING", ""))
        records = load_records()

        if filters:
            filtered = [r for r in records if record_matches(r, filters)]
            render_records("GET records (filtered)", filtered[-20:])
        else:
            render_records("GET recent records", records[-20:])

    else:
        print(f"<h2>Unsupported method: {html_escape(method)}</h2>")

if __name__ == "__main__":
    main()
