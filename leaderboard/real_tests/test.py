import requests
from time import time 

def test():
    r = requests.get('http://127.0.0.1:8888/api/score/setState?state=registration')
    print(r)
    assert r.status_code == 200

    r = requests.get('http://127.0.0.1:8888/api/score/setState?state=starth')
    print(r)
    assert r.status_code == 400

    r = requests.get('http://127.0.0.1:8888/api/score/register')
    print(r)
    assert r.status_code == 200

    r = requests.get('http://127.0.0.1:8888/api/score/start')
    print(r)
    # start should give -1 if game has not been scheduled to start
    assert r.json() == -1

    r = requests.get('http://127.0.0.1:8888/api/score/setState?state=start')
    print(r)
    assert r.status_code == 200

    r = requests.get('http://127.0.0.1:8888/api/score/start')
    print(r)
    # allow for a couple second leniency, but must be in at least 5 seconds ahead, as server thinks it is 10 seconds ahead
    assert r.json() > time() + 5

    print('all tests passed!')

if __name__ == '__main__':
    test()