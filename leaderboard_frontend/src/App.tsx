import { LeaderboardPage } from "./Leaderboard";

const App = () => (
  <>
    {/* can add routing here if we want other things */}
    <LeaderboardPage
      style={{
        width: "90%",
        maxWidth: 800,
        margin: "auto",
        marginBottom: 40,
        marginTop: 40,
      }}
    />
  </>
);

export default App;
