import {
  Grid,
  GridProps,
  Paper,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  Typography,
} from "@mui/material";
import { orderBy, startCase, countBy } from "lodash-es";
import currentGameData from "./data/current-game.json";
import { GameController } from "./GameController";

type Player = { name: string; taggedBy: string[] };
type Players = { [id: string]: Pick<Player, "taggedBy"> };
type Column = Player & {
  points: number;
  tags: string[];
};

const playersToColumns = (players: Player[]): Column[] => {
  const tags: Record<string, string[]> = {};

  // convert taggedBy to tagged, a list of who was tagged by who to a list of who tagged who
  players.forEach((player) => {
    player.taggedBy.forEach((tagger) => {
      tags[tagger] = [...(tags[tagger] ?? []), player.name];
    });
  });

  return orderBy(
    players.map((player) => {
      const playerTags = tags[player.name] ?? [];

      return {
        ...player,
        tags: playerTags,
        points: playerTags.length * 10 - player.taggedBy.length * 3,
      };
    }),
    ["points"],
    ["desc"]
  );
};

const tableColumns: {
  [Property in keyof Column]: boolean;
} = {
  name: true,
  tags: true,
  taggedBy: true,
  points: true,
};

export const LeaderboardPage = (props: GridProps) => {
  // cast to players to unknown first cuz weird typescript stuff
  const playerData = currentGameData as unknown as Players;

  let playerIdToNumberMap: Record<string, string> = {};

  const players = Object.entries(playerData).map(([id, { taggedBy }], i) => {
    // convert ids to player {id}
    const formattedPlayer: Player = {
      taggedBy: taggedBy.map((taggerId) => taggerId),
      name: id,
    };

    playerIdToNumberMap[id] = `${i + 1}`;
    return formattedPlayer;
  });

  const displayPlayerName = (playerId: string) => {
    return `Player ${playerIdToNumberMap[playerId]}`;
  };

  const columns = playersToColumns(players);

  const formatTags = (tags: string[]) => (
    <>
      <Typography variant="body1">{tags.length}</Typography>
      {tags.length > 0 && (
        <Typography variant="caption">
          (
          {Object.entries(countBy(tags))
            .map(
              ([tagger, tagCount]) =>
                `${displayPlayerName(tagger)}: ${tagCount}`
            )
            .join(", ")}
          )
        </Typography>
      )}
    </>
  );

  const Leaderboard = ({ columns }: { columns: Column[] }) => (
    <TableContainer component={Paper}>
      <Table>
        <TableHead>
          <TableRow>
            {Object.keys(tableColumns).map((tableColumn, i) => (
              <TableCell
                align={i === 0 ? undefined : "center"}
                key={tableColumn}
              >
                {startCase(tableColumn)}
              </TableCell>
            ))}
          </TableRow>
        </TableHead>

        <TableBody>
          {columns.map((player, i) => (
            <TableRow key={player.name}>
              <TableCell>
                <Typography variant="button">
                  {displayPlayerName(player.name)}
                </Typography>
              </TableCell>
              <TableCell align="center">{formatTags(player.tags)}</TableCell>
              <TableCell align="center">
                {formatTags(player.taggedBy)}
              </TableCell>
              <TableCell align="center">
                <Typography variant="h6">{player.points}</Typography>
              </TableCell>
            </TableRow>
          ))}
        </TableBody>
      </Table>
    </TableContainer>
  );

  return (
    <Grid container justifyContent="center" {...props} spacing={6}>
      <Grid item>
        <GameController />
      </Grid>
      <Grid item xs={12}>
        <Leaderboard columns={columns} />
      </Grid>
    </Grid>
  );
};
