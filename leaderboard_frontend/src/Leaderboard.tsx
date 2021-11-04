import {
  Button,
  Card,
  CardActions,
  CardContent,
  CardHeader,
  Grid,
  GridProps,
  MenuItem,
  Paper,
  Select,
  Slider,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  Typography,
} from "@mui/material";
import { orderBy, startCase, countBy, random, maxBy } from "lodash-es";
import { useMemo, useState } from "react";

type Player = { name: string; taggedBy: string[] };
type Column = Player & {
  points: number;
  tags: string[];
};

const playersToColumns = (players: Player[]): Column[] => {
  const tags: Record<string, string[]> = {};

  players.forEach((player) => {
    player.taggedBy.forEach((tagger) => {
      tags[tagger] = [...(tags[tagger] ?? []), player.name];
    });
  });

  return players.map((player) => {
    const playerTags = tags[player.name] ?? [];

    return {
      ...player,
      tags: playerTags,
      points: 20 + playerTags.length * 10 - player.taggedBy.length * 3,
    };
  });
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

const tableColumns: {
  [Property in keyof Column]: boolean;
} = {
  name: true,
  tags: true,
  taggedBy: true,
  points: true,
};

const players = ["Alex", "Andy", "Jason", "David", "Ian"];
const getRandomAssortment = (name: string) => {
  const notSelf = players.filter((playerName) => playerName !== name);
  const numToGenerate = random(0, 30);

  const generated = [];
  for (let i = 0; i < numToGenerate; i++) {
    const index = random(notSelf.length - 1);
    generated.push(notSelf[index]);
  }
  return generated;
};

const generateColumns = () =>
  orderBy(
    playersToColumns([
      {
        name: "Andy",
        taggedBy: getRandomAssortment("Andy"),
      },
      {
        name: "Alex",
        taggedBy: getRandomAssortment("Alex"),
      },
      {
        name: "Ian",
        taggedBy: getRandomAssortment("Ian"),
      },
      {
        name: "David",
        taggedBy: getRandomAssortment("David"),
      },
      {
        name: "Jason",
        taggedBy: getRandomAssortment("Jason"),
      },
    ]),
    ["points"],
    ["desc"]
  );

const getHighestGeneration = (trials: number, maximize = true) => {
  const generated = [...new Array(trials)].map(() => generateColumns());
  console.log("generated", generated.length);
  return (
    maxBy(generated, (x) => (maximize ? x[0].points : -1 * x[0].points)) ?? []
  );
};

export const LeaderboardPage = ({
  players,
  ...props
}: { players: Player[] } & GridProps) => {
  const defaultTrials = 1;

  const initialTrials = useMemo(
    () => getHighestGeneration(defaultTrials),
    [defaultTrials]
  );

  const [maxTrials, setMaxTrials] = useState(defaultTrials);
  const [maximizeScore, setMaximizeScore] = useState(true);

  const [columns, setColumns] = useState(initialTrials);

  return (
    <>
      <Grid container justifyContent="center" {...props}>
        <Grid item>
          <Card
            style={{
              marginTop: 30,
              marginBottom: 30,
              paddingLeft: 10,
              paddingRight: 10,
            }}
            variant="outlined"
          >
            <CardHeader title="Stimulating Simulation Station" />
            <CardContent>
              <Typography variant="body1">
                This will run <i>n</i> simulations of the game and take the
                highest or lowest scoring trial
              </Typography>
              <br />

              <Typography variant="body2">
                Number of simulations to run (makes min/max more dramatic)
              </Typography>
              <Slider
                min={1}
                max={1000}
                defaultValue={defaultTrials}
                onChange={(_, newValue) => setMaxTrials(newValue as number)}
                valueLabelDisplay="auto"
              />
              <Typography variant="body2">
                Maximize or minimize score
              </Typography>
              <Select
                onChange={(e) => setMaximizeScore(e.target.value === "max")}
                defaultValue="max"
                size="small"
              >
                <MenuItem value="max">Max</MenuItem>
                <MenuItem value="min">Min</MenuItem>
              </Select>
              <br />
              <br />
              <Typography variant="body2">Generate new data</Typography>
            </CardContent>
            <CardActions>
              <Button size="small" color="inherit">
                no lmao
              </Button>
              <Button
                size="small"
                color="primary"
                onClick={() =>
                  setColumns(getHighestGeneration(maxTrials, maximizeScore))
                }
              >
                sure
              </Button>
            </CardActions>
          </Card>
        </Grid>

        <Grid item xs={12}>
          <Leaderboard columns={columns} />
        </Grid>
      </Grid>
    </>
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
