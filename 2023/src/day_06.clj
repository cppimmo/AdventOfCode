(ns day-06
  "Day 6: Wait For It"
  (:require aoc
            [clojure.string :as str]))

(def input (aoc/read-input 6))
(def test-input (aoc/read-input 6 :test))

;;; Time allow for each race (milliseconds)
;;; Best distance during the race (millimeters)
;;; I need to go farther than current record holder

(defn read-races
  [part input]
  (let [races (mapv (fn [line]
                      (cond
                        (= part :one) (aoc/str->numbers
                                       (str/replace line #"[A-Za-z:]" ""))
                        (= part :two) (parse-long
                                       (str/replace line #"[\sA-Za-z:]" ""))
                        :else (assert false "Only :one & :two are accepted for part!")))
                    input)]
    (if (= part :one)
      (apply mapv (fn [time dist]
                    {:time time :distance dist})
             races)
      (vector {:time (first races) :distance (second races)}))))

(defn ways-to-win
  [races]
  (mapv (fn [{:keys [time distance]}]
          (let [button-times (range 0 (inc time))]
            (count ; Want the count of winning button times
             (filter (fn [t]
                       (let [own-dist (* (- time t) t)] ; My own distance traveled
                         (> own-dist distance))) ; Needs to be more than competitor
                     button-times))))
        races))

(defn solve
  [input]
  (reduce * (->> input (read-races :one) ways-to-win)))

(defn solve-2
  [input]
  (first (->> input (read-races :two) ways-to-win)))

(println "Answer 1:" (solve input))
(println "Answer 2:" (solve-2 input))

