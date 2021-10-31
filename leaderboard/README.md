# Laser Tag</br>Leaderboard Server
![Badge indicating this is a Node.js v16 project.](https://img.shields.io/badge/node.js-16-blue)

![Cute Clip Art of a chalkboard.](https://lh3.googleusercontent.com/proxy/uncC8TwJnzGX1wuk17BcacyndjJzGuH_YQfw8vgwDEH9pQeZzh_8mrby-uDzVbTSA-gRaSsUBGbJLNKd1B44ykKaFOtAMxvvrlPqpA)

## Description

This contains a server that both receives communication from the Laser Tag game and displays a leaderboard that keeps track of players' scores.


## How to setup

### You will need:

* [Node v16](https://nodejs.org/en/)

### Setup instructions

1. Navigate to this directory (`leaderboard/`).
2. In a terminal, run
```bash
npm install
```

---

### How to run

```bash
npm run start
```

* To simulate a call when you fire your weapon, hit the endpoint "`http://localhost:8888/api/score/fired?id=my_id`".
* To simulate a call when your vest was hit, hit the endpoint "`http://localhost:8888/api/score/hit?id=my_id`".
