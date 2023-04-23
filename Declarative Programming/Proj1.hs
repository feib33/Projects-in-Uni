--  File       : Proj1.hs
--  Author     : Fei Bao
--  Student ID : 804522
--  Origin     : Aug 31 2018
--  Purpose    : Implement the guessing part of a logical guessing game

--
--  This file implements initialGuess and nextGuess functions to determine 
--  which suspects to include in each lineup and try to find out 
--  all suspects (2) within the minimium times by choosing the possible 
--  lineup which leading to leave a small remaining list of possible
--  lineups. 
--  


module Proj1 (Person, parsePerson, height, hair, sex, 
             GameState, initialGuess, nextGuess, feedback) where

import Data.List

--  Person is composed of 3 single Chars type : height, hair and gender
--  GameState is a list of remaining possible lineup which contains
--  2 people
data Person = Person Char Char Char deriving (Eq, Show, Ord)
type GameState = [[Person]]


--  Transfer IO input String into a data type of Person
--  and return Noting if input is invalid
parsePerson :: String -> Maybe Person
parsePerson "" = Nothing
parsePerson p = Just (Person (p !! 0) (p !! 1) (p !! 2))


--  Extract height data from a person
height :: Person -> Char
height (Person x _ _) = x


--  Extract hair colour data from a person
hair :: Person -> Char
hair (Person _ x _) = x


--  Extract gender data from a person
sex :: Person -> Char
sex (Person _ _ x) = x


--  Input a list of true culprits and a list of suspects
--  and receive the feedback from the witnesses comprise
--  four numbers: the number of identified culprits, the 
--  number with the right height, the number with the right 
--  hair colour and the number with the right sex
feedback :: [Person] -> [Person] -> (Int, Int, Int, Int)
feedback target guess = (correctCul, correctHeight, correctHair, correctSex) 
    where
    	-- extract same person between target and guess
        samePerson = target `intersect` guess
        correctCul = length samePerson

        -- extract height/hair/sex from guess and target
        -- and compare each other
        tagHeight  = map height (target \\ samePerson)
        gusHeight  = map height (guess \\ samePerson)
        correctHeight = length tagHeight - 
                            length (tagHeight \\ gusHeight)

        tagHair  = map hair (target \\ samePerson)
        gusHair  = map hair (guess \\ samePerson)
        correctHair = length tagHair - 
                            length (tagHair \\ gusHair)

        tagSex  = map sex (target \\ samePerson)
        gusSex  = map sex (guess \\ samePerson)
        correctSex = length tagSex - 
                            length (tagSex \\ gusSex)
        


-- Return a initial guess which is a tuple comprise 2 elments:
-- first optimal lineup and the current information of game -
-- all possible remaining pairs. Note that initial pair 
-- [SBM, SRM] should be most optimal initial guess
initialGuess :: ([Person], GameState)
initialGuess = (iniPair, iniGameState)
    where 
        iniPair = [Person 'S' 'B' 'M', Person 'S' 'R' 'M']
        iniGameState = delete iniPair allPairs 


-- Return next guess based on previous guess and the feedback 
-- according to the rule which the guess has most least remaining
-- possible lineups (GameState). GameState should be updated by 
-- removing some pairs that are inconsistent with previous guess
nextGuess :: ([Person], GameState) -> (Int, Int, Int, Int) 
                                              -> ([Person], GameState)
nextGuess (prevGuess, gameState) answer = (newGuess, updatedGameState)
    where 
        consisGameState = delInconsistent (prevGuess, gameState) answer
        newGuess = optimalLineup consisGameState
        updatedGameState = delete newGuess consisGameState


-- Delete lineups which return different feedback with previous guess 
-- in the GameState
delInconsistent :: ([Person], GameState) -> (Int, Int, Int, Int) -> GameState
delInconsistent (prev, gameState) answer
    = filter (\x -> feedback x prev == answer) gameState


-- Compute average number of possible lineups that will remain
-- after each lineup.
avgRmCands :: ([Person], GameState) -> Double
avgRmCands (possTarget, gameState) =
    sum [(fromIntegral numCands / fromIntegral totNumCands)
        * fromIntegral numCands | numCands <- sameOputGroup]
        where
            -- group and sort lineups in the gameState
            -- by yielding the same output
            sameOputGroup = map length$ group $ sort $ 
                                map (feedback possTarget) gameState
            totNumCands = sum sameOputGroup


-- Generate all possible lineups which contains exactly different person
allPairs :: [[Person]]
allPairs = nubBy (\x y -> x == reverse y) [[firTarget, secTarget] 
                                          |firTarget <- allPeople
                                          ,secTarget <- [x | x <- allPeople
                                          , x /= firTarget]]
                                          where 
                                              allPeople = 
                                                [Person heit hair sex 
                                                | heit <- "ST"
                                                , hair <- "BRD"
                                                , sex <- "MF"]


-- Get the most optimal lineup which has the lowest average
-- remaining possible lineups in the gameState
optimalLineup :: GameState -> [Person]
optimalLineup gameState = opLineup
    where
        lineups  = [(avg, lineup) 
                   | lineup <- gameState
                   , let avg = avgRmCands (lineup,(delete lineup gameState))]
        opLineup = snd $ foldl1 min lineups