import { LeaderboardPage } from "./Leaderboard";

const App = () => (
  <>
    {/* can add routing here if we want other things */}
    <LeaderboardPage
      //  haven't figured out how we should pass the players in, perhaps read from local file or make a request or something
      players={[]}
      style={{ width: "90%", maxWidth: 800, margin: "auto", marginBottom: 40 }}
    />
  </>
);

export default App;
