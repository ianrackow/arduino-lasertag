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
        points: 20 + playerTags.length * 10 - player.taggedBy.length * 3,
      };
    }),
    ["points"],
    ["desc"]
  );
};

const formatTags = (tags: string[]) => (
  <>
    <Typography variant="body1">{tags.length}</Typography>
    {tags.length > 0 && (
      <Typography variant="caption">
        (
        {Object.entries(countBy(tags))
          .map(([tagger, tagCount]) => `${tagger}: ${tagCount}`)
          .join(", ")}
        )
      </Typography>
    )}
  </>
);

const formatPlayerName = (name: string) => `Player ${name}`;

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

  const players = Object.entries(playerData).map(([id, { taggedBy }]) => {
    // convert ids to player {id}
    const formattedPlayer: Player = {
      taggedBy: taggedBy.map((taggerId) => formatPlayerName(taggerId)),
      name: formatPlayerName(id),
    };
    return formattedPlayer;
  });

  const columns = playersToColumns(players);

  return (
    <Grid container justifyContent="center" {...props}>
      <Grid item xs={12}>
        <Leaderboard columns={columns} />
      </Grid>
    </Grid>
  );
};

export const Leaderboard = ({ columns }: { columns: Column[] }) => (
  <TableContainer component={Paper}>
    <Table>
      <TableHead>
        <TableRow>
          {Object.keys(tableColumns).map((tableColumn, i) => (
            <TableCell align={i === 0 ? undefined : "center"} key={tableColumn}>
              {startCase(tableColumn)}
            </TableCell>
          ))}
        </TableRow>
      </TableHead>

      <TableBody>
        {columns.map((player) => (
          <TableRow key={player.name}>
            <TableCell>
              <Typography variant="button">{player.name}</Typography>
            </TableCell>
            <TableCell align="center">{formatTags(player.tags)}</TableCell>
            <TableCell align="center">{formatTags(player.taggedBy)}</TableCell>
            <TableCell align="center">
              <Typography variant="h6">{player.points}</Typography>
            </TableCell>
          </TableRow>
        ))}
      </TableBody>
    </Table>
  </TableContainer>
);
