-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Tempo de geração: 14-Dez-2023 às 03:30
-- Versão do servidor: 10.4.22-MariaDB
-- versão do PHP: 7.4.27

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Banco de dados: `n2Passagens`
--

-- --------------------------------------------------------

--
-- Estrutura da tabela `viagens`
--

CREATE TABLE `viagens` (
  `idViagem` bigint(20) NOT NULL,
  `dataViagem` date NOT NULL,
  `assento` enum('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26','27','28','29','30','31','32','33','34','35','36','37','38','39','40','41','42','43','44','45') DEFAULT NULL,
  `nomePassageiro` varchar(100) NOT NULL,
  `fortalezaQuixada` tinyint(1) NOT NULL,
  `quixadaIguatu` tinyint(1) NOT NULL,
  `iguatuJuazeiro` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Extraindo dados da tabela `viagens`
--

INSERT INTO `viagens` (`idViagem`, `dataViagem`, `assento`, `nomePassageiro`, `fortalezaQuixada`, `quixadaIguatu`, `iguatuJuazeiro`) VALUES
(1, '2023-11-01', '10', 'Alberto', 1, 0, 0),
(2, '2023-11-01', '20', 'Bruna', 1, 1, 0),
(3, '2023-11-01', '30', 'Carlos', 0, 1, 1),
(4, '2023-11-01', '40', 'Daniela', 1, 1, 1),
(5, '2023-11-01', '45', 'Eudes', 0, 0, 1),
(6, '2023-11-01', '1', 'Francisca', 0, 1, 0),
(7, '2023-11-01', '11', 'George', 1, 0, 0),
(8, '2023-11-01', '21', 'Helena', 1, 1, 0),
(9, '2023-11-01', '31', 'Igor', 0, 1, 1),
(10, '2023-11-01', '41', 'José', 1, 1, 1),
(11, '2023-11-01', '2', 'Karina', 0, 0, 1),
(12, '2023-11-01', '12', 'Marcos', 0, 1, 0),
(13, '2023-11-01', '22', 'José', 1, 1, 1),
(14, '2023-11-01', '32', 'Lorena', 0, 0, 1),
(15, '2023-11-01', '42', 'Natan', 0, 1, 0),
(16, '2023-11-01', '3', 'Ofélia', 1, 0, 0),
(17, '2023-11-01', '13', 'Paulo', 1, 1, 0),
(18, '2023-11-01', '23', 'Quênia', 0, 1, 1),
(19, '2023-11-01', '33', 'Raul', 1, 1, 1),
(20, '2023-11-01', '43', 'Sara', 0, 0, 1),
(21, '2023-11-01', '4', 'Tales', 0, 1, 0),
(22, '2023-11-01', '14', 'Úrsula', 1, 1, 1),
(23, '2023-11-01', '24', 'Vítor', 0, 0, 1);

--
-- Índices para tabelas despejadas
--

--
-- Índices para tabela `viagens`
--
ALTER TABLE `viagens`
  ADD PRIMARY KEY (`idViagem`),
  ADD UNIQUE KEY `assento` (`assento`);

--
-- AUTO_INCREMENT de tabelas despejadas
--

--
-- AUTO_INCREMENT de tabela `viagens`
--
ALTER TABLE `viagens`
  MODIFY `idViagem` bigint(20) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=42;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
