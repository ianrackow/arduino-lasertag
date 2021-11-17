"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    Object.defineProperty(o, k2, { enumerable: true, get: function() { return m[k]; } });
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const crypto_1 = require("crypto");
const express_1 = __importDefault(require("express"));
const fs_1 = require("fs");
const path_1 = __importStar(require("path"));
let game;
const gamePath = "../data/current-game.json";
const router = express_1.default.Router();
const dataDir = path_1.default.normalize("../data");
const currentGame = path_1.default.join(dataDir, "current-game.json");
const pastGamesDir = path_1.default.join(dataDir, "past-games");
router.get("/", (_, res) => {
    res.json({ msg: "🙆" });
});
router.get("/start", (_, res) => {
    console.log("data_dir", dataDir, currentGame);
    // if there is a current game, rename it to the hash of the file cuz I don't know how else to generate a unique filename
    if ((0, fs_1.existsSync)(currentGame)) {
        const data = (0, fs_1.readFileSync)(currentGame);
        const hashSum = (0, crypto_1.createHash)("sha256");
        hashSum.update(data);
        const currentGameHash = hashSum.digest("hex");
        (0, fs_1.renameSync)(currentGame, path_1.default.join(dataDir, currentGameHash));
    }
    else {
        console.log("lmao current game no exist stupid");
    }
    res.sendStatus(200);
});
router.get("/hit", (req, res) => {
    var _a, _b;
    /**
     * For now, this router will receive data in the format
     * "id={ID}" to indicate that the player of the specified ID was hit.
     */
    console.log(req.query);
    console.log(req.query.id);
    const id = req.query.id;
    const taggerId = req.query.taggerId;
    if (typeof id === "string" && typeof taggerId === "string") {
        const data = ((_a = require("../../data/current-game.json")) !== null && _a !== void 0 ? _a : {});
        const { taggedBy } = (_b = data[id]) !== null && _b !== void 0 ? _b : { taggedBy: [] };
        const newPlayerData = { taggedBy: [...taggedBy, taggerId] };
        const newData = Object.assign(Object.assign({}, data), { [id]: newPlayerData });
        console.log("data", data);
        console.log("other path", (0, path_1.resolve)("data/current-game.json"));
        (0, fs_1.writeFileSync)("data/current-game.json", JSON.stringify(newData));
        res.sendStatus(200);
    }
    else {
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
exports.default = router;
//# sourceMappingURL=score.js.map