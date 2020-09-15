# AI for 8x8 checkers

This repository integrates with [Checkers web site](github.com/Aleksej10/checkers)

## Dependencies

- C++ compiler with `CXX_STANDARD 17 support`
- `Cmake >= 3.16.5`
- [PyTorch C++](pytorch.org/cppdocs/installing.html)
- [WebSocket++](docs.websocketpp.org)
- [nlohmann/json](github.com/nlohmann/json) (already included)

## Algorithm

AI uses CNN for estimating evaluation of a position.  
Search algorithm is a modification of Monte Carlo Tree Search.  
Trained model can be found on [Checkers web site](github.com/Aleksej10),  
as well as the binary (target: `x86_64-pc-linux-gnu`).

## About
* Authors:     see the `LICENSE`
* License:     MIT License
* Bug reports: https://github.com/Aleksej10/checkersAI/issues
* git clone    https://github.com/Aleksej10/checkersAI
