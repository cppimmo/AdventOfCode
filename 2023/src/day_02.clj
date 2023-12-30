(ns advent-of-code-2023.day-02
  (:require [clojure.java.io :as io]
            [clojure.string :as str]))

(def filename "resources/day_02.txt")

(def bag-cubes
  "Map of possible cubes counts."
  {:red 12
   :green 13
   :blue 14})

(defn possible?
  "Determine if the map, cubes, represents a valid round."
  [cubes]
  (every? identity ; Each element is a boolean itself
          (map (fn [color]
                 (<= (cubes color 0) (bag-cubes color 0))) ; Check for invalid cube count
               (keys bag-cubes)))) ; Seq of keys in map

(defn possible-game?
  "Determine if the map of cubes from a game is valid."
  [game]
  (every? identity
          (map possible? game)))

;;; Read input may look like ([id {:red _ :green _ :blue _}] ...)

(defn get-game-id
  [s]
  (->> (subs s 0 (or (str/index-of s ":") (count s)))
       (re-seq #"\d+")
       (first)
       (Integer/parseInt)))

(defn get-cubes
  [s]
  (map (fn [elem]
         (reduce #(merge-with + %1 %2)
                 (map (fn [cube]
                        (let [[count color] (str/split (str/trim cube) #"\s+")]
                          {(keyword color) (Integer/parseInt count)}))
                      elem)))
       (map #(str/split (str/trim %) #",")
            (str/split (subs s (inc (str/index-of s ":"))) #";"))))

(defn get-game-status
  [s]
  (let [id (get-game-id s)
        cubes (get-cubes s)]
    (vector id cubes)))

(defn read-input
  [^String f]
  (with-open [rdr (io/reader f)]
    (->> (line-seq rdr)
         (doall))))

(comment
  (def test-games
    (list "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
          "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue"
          "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red"
          "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red"
          "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green")))

(defn minimums
  [cubes]
  (->> cubes
       (reduce (fn [acc cube]
                 (merge-with max acc cube)))))

(defn power
  [cubes]
  (reduce * (vals cubes)))

(defn solve
  []
  (reduce (fn [acc status]
            (+ acc (first status)))
          0
          (filter #(possible-game? (second %)) (map get-game-status
                                                  (read-input filename)))))

(defn solve-2
  []
  (reduce (fn [acc cubes]
            (+ acc (power (minimums cubes))))
          0
          (map (fn [line]
                 (second (get-game-status line)))
               (read-input filename))))

(println "Answer 1:" (solve))
(println "Answer 2:" (solve-2))

