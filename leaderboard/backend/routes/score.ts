import { createHash } from "crypto";
import express from "express";
import { existsSync, readFileSync, renameSync, writeFileSync } from "fs";
import path, { resolve } from "path";

type Player = { taggedBy: string[] };
type Players = { [id: string]: Player | undefined };

let game;

const gamePath = "../data/current-game.json";

const router = express.Router();
const dataDir = path.normalize("../data");
const currentGame = path.join(dataDir, "current-game.json");
const pastGamesDir = path.join(dataDir, "past-games");

router.get("/", (_, res) => {
  res.json({ msg: "🙆" });
});

router.get("/start", (_, res) => {
  console.log("data_dir", dataDir, currentGame);

  // if there is a current game, rename it to the hash of the file cuz I don't know how else to generate a unique filename
  if (existsSync(currentGame)) {
    const data = readFileSync(currentGame);
    const hashSum = createHash("sha256");
    hashSum.update(data);

    const currentGameHash = hashSum.digest("hex");

    renameSync(currentGame, path.join(dataDir, currentGameHash));
  } else {
    console.log("lmao current game no exist stupid");
  }

  res.sendStatus(200);
});

router.get("/hit", (req, res) => {
  /**
   * For now, this router will receive data in the format
   * "id={ID}" to indicate that the player of the specified ID was hit.
   */
  console.log(req.query);
  console.log(req.query.id);
  const id = req.query.id as string | undefined;
  const taggerId = req.query.taggerId as string | undefined;

  if (typeof id === "string" && typeof taggerId === "string") {
    const data = (require("../../data/current-game.json") ?? {}) as Players;

    const { taggedBy } = data[id] ?? { taggedBy: [] };
    const newPlayerData: Player = { taggedBy: [...taggedBy, taggerId] };

    const newData: Players = { ...data, [id]: newPlayerData };

    console.log("data", data);
    console.log("other path", resolve("data/current-game.json"));

    writeFileSync("data/current-game.json", JSON.stringify(newData));

    res.sendStatus(200);
  } else {
    res.sendStatus(400);
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
