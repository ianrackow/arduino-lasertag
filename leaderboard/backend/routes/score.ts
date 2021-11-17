import { createHash } from "crypto";
import express from "express";
import { copyFileSync, existsSync, readFileSync, writeFileSync } from "fs";
import path from "path";

type Player = { taggedBy: string[] };
type Players = { [id: string]: Player | undefined };

const router = express.Router();

router.get("/", (_, res) => {
  res.json({ msg: "🙆" });
});

const currentGameFile = path.posix.join("data", "current-game.json");
const pastGamesDir = path.posix.join("data", "past-games");

router.get("/start", (_, res) => {
  // if there is a current game, rename it to the hash of the file cuz I don't know how else to generate a unique filename
  if (existsSync(currentGameFile)) {
    const data = readFileSync(currentGameFile);

    // if there is data, copy it to past games just for funsies
    if (data) {
      const hashSum = createHash("sha256");
      hashSum.update(data);

      const currentGameHash = hashSum.digest("hex");

      copyFileSync(currentGameFile, path.join(pastGamesDir, currentGameHash));
    }
  } else {
    console.log("lmao current game no exist stupid");
  }

  writeFileSync(currentGameFile, JSON.stringify({}));

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
    // current-game.json could be empty, so return an empty array if true
    const data = (require("../../data/current-game.json") || {}) as Players;

    const { taggedBy } = data[id] ?? { taggedBy: [] };
    const newPlayerData: Player = { taggedBy: [...taggedBy, taggerId] };

    const newData: Players = { ...data, [id]: newPlayerData };

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
