import argparse
import os
import random
import time
from typing import List

import requests
from bs4 import BeautifulSoup

BASE_URL = "https://codeforces.com"

parser = argparse.ArgumentParser(
    description="Script to scrape codes from Codeforces for the plagiarism software checks")
parser.add_argument("problem", type=str, help="A problem number. Write number_letter, e.g. 1607_E")
parser.add_argument("-f", "--from", dest="fr", type=int, help="A starting page", default=1)
parser.add_argument("-t", "--to", dest="to", type=int, help="Ending page")
parser.add_argument("-r", "--random", dest="rand", action="store_true",
                    help="Takes from 5 to 25 random pages in the given page range")
parser.add_argument("-rf", "--rand_from", dest="rf", type=int, help="A random range starting number", default=5)
parser.add_argument("-rt", "--rand_to", dest="rt", type=int, help="A random range ending number", default=25)

args = parser.parse_args()

# contains problem number and letter
problem: List[str] = args.problem.split("_")
from_page = args.fr
to_page = args.to

# get last page
if to_page is None:
    url = f"{BASE_URL}/problemset/status/{problem[0]}/problem/{problem[1]}"
    req = requests.get(url)
    soup = BeautifulSoup(req.text, "html.parser")
    to_page = int(soup.find_all("span", {"class": "page-index"})[-1].find("a").text)
    print(to_page)

if not os.path.exists("datasets"):
    os.makedirs("datasets")


def download_file(file_path, link, page):
    req = requests.get(link)
    soup = BeautifulSoup(req.text, "html.parser")
    code = soup.find("pre", {"id": "program-source-text"}).text.replace("\r\n", "\n")
    with open(file_path, "w", encoding="utf-8") as f:
        f.write(code)
    print(f"File {file_path} is downloaded from page: {page}")
    time.sleep(0.1)


if args.rand:
    count = random.randint(args.rf, args.rt)
    pages = set([random.randint(from_page, to_page) for _ in range(count)])
else:
    pages = range(from_page, to_page + 1)
print(f"Iterating over pages: {pages}")

for page in pages:

    url = f"https://codeforces.com/problemset/status/{problem[0]}/problem/{problem[1]}/page/{page}"

    req = requests.get(url)
    html = req.text

    soup = BeautifulSoup(html, "html.parser")
    # get table rows
    rows = soup.find("table", {"class": "status-frame-datatable"}).find_all("tr")

    splitted = url.split("/")
    dirname = splitted[5] + splitted[7]
    problem_path = f"./datasets/{dirname}"
    if not os.path.exists(problem_path):
        os.makedirs(problem_path)

    # skip first empty row
    for i in range(1, len(rows)):
        cells = rows[i].find_all("td")
        # get link to the submission page
        link = cells[0].find("a")
        language = cells[4].text
        problem_id = link["submissionid"]

        if language.strip()[:4] == "Java":
            url = BASE_URL + link["href"]
            filename = f"{problem_id}.java"
            download_file(f"{problem_path}/{filename}", url, page)

    time.sleep(random.uniform(0.1, 0.8))
