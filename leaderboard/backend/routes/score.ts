import express from "express";
import { writeFileSync } from "fs";
import moment, { Moment } from "moment";
import path from "path";

type Player = { taggedBy: string[] };
type Players = { [id: string]: Player | undefined };

const router = express.Router();

router.get("/", (_, res) => {
  res.json({ msg: "🙆" });
});

const currentGameFile = path.posix.join("data", "current-game.json");

// variable for players who have called start
enum State {
  Registration = "registration",
  Start = "start",
  GameOver = "gameOver",
}

let state: State = State.GameOver;
let registeredPlayers: Players = {};
let startTime: Moment = moment();

const syncRegisteredPlayers = () =>
  writeFileSync(currentGameFile, JSON.stringify(registeredPlayers));

const addRegisteredPlayer = (playerId: string) =>
  (registeredPlayers[playerId] = { taggedBy: [] });

router.get("/setState", (req, res) => {
  const status = req.query.state as string;

  console.log("setState", status);

  let returnStatus: string | number = 200;
  if (status === State.Registration) {
    state = State.Registration;
    registeredPlayers = {};
    syncRegisteredPlayers();
  } else if (status === State.Start) {
    state = State.Start;
    startTime = moment().add(10, "seconds");
  } else if (status === State.GameOver) {
    state = State.GameOver;
  } else {
    returnStatus = "bad state";
  }
  res.json(returnStatus);
});

router.get("/start", (_, res) => {
  if (state === State.Registration) {
    res.json(-1);
  } else if (state === State.Start) {
    res.json(startTime.unix());
  }
});

router.get("/register", (req, res) => {
  console.log(state);
  if (state === State.Registration) {
    const playerId = (req.query.id as string | undefined) ?? req.ip;

    console.log("adding player", playerId);
    addRegisteredPlayer(playerId);

    res.sendStatus(200);

    syncRegisteredPlayers();
  } else {
    res.json("registration not enabled");
  }
});

router.get("/hit", (req, res) => {
  /**
   * For now, this router will receive data in the format
   * "id={ID}" to indicate that the player of the specified ID was hit.
   */
  const id = req.ip;
  const [taggerId, _] = Object.entries(registeredPlayers).find(
    ([playerId, _]) => id !== playerId
  ) ?? [undefined, undefined];

  if (taggerId === undefined) {
    res.json("cannot determine a taggerId");
  } else {
    const { taggedBy } = registeredPlayers[id] ?? { taggedBy: [] };
    const newPlayerData: Player = { taggedBy: [...taggedBy, taggerId] };

    registeredPlayers = { ...registeredPlayers, [id]: newPlayerData };

    res.sendStatus(200);

    syncRegisteredPlayers();
  }
});

router.get("/fired", (req, res) => {
  /**
   * For now, this router will receive data in the format
   * "id={ID}" to indicate that the player of the specified ID shot.
   */
  console.log(req.query);
  console.log(req.query.id);
  res.send("ok");
});

export default router;
