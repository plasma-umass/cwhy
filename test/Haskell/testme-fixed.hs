import Graphics.Gnuplot.Simple

plot1 :: IO ()
plot1 = plotFunc [] [0, 0.01 .. 10::Double] cos

main :: IO ()
main = plot1
