-- 找到每天排名前 5 的国家及其人口和 GDP 排名
SELECT
    r.date AS date,                                 -- 比赛日期
    c.code AS country_code,                        -- 国家代码
    COUNT(*) AS top5_appearances,                 -- 国家在前 5 名的出现次数
    c."GDP ($ per capita)" AS gdp_rank,            -- 国家 GDP 排名
    c.Population AS population_rank               -- 国家人口排名
FROM
    results r
    JOIN
    athletes a
ON r.participant_code = a.code                 -- 比赛参与者是运动员
    OR r.participant_code IN (                     -- 或者是团队
    SELECT code
    FROM teams
    WHERE athletes_code = a.code
    )
    JOIN
    countries c
    ON c.code = a.country_code                     -- 获取参与者代表的国家
WHERE
    r.rank IS NOT NULL                             -- 排名不为空
  AND r.rank <= 5                                -- 限制条件：排名在前 5 名
GROUP BY
    r.date, c.code                                 -- 按日期和国家分组
ORDER BY
    r.date ASC,                                    -- 按日期升序排序
    top5_appearances DESC,                         -- 前 5 次数降序排序
    c.code ASC;                                    -- 如果次数相同，按国家代码升序排序
